// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsCharacter.h"

#include"MyCharacterStatComponent.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayersComponent/MotionReplicatorInterface.h"
#include "Cars/MyProjectPawn.h"
#include "PuzzlePlatformsGameInstance.h"
#include "MyPlayerController.h"
#include "PlayerInfoWidget.h"
#include "PlayersComponent/SoldierMotionReplicator.h"
#include "MyLobbyGameMode.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"
#include "Components/SphereComponent.h"


#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// APuzzlePlatformsCharacter


FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Error";
	}
}
APuzzlePlatformsCharacter::APuzzlePlatformsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	//추가하는 코드
	SetActorTickEnabled(true);
	bReplicates = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//IsAttacking = false;
	//DaerimMotionReplicator = CreateDefaultSubobject<USoldierMotionReplicator>(TEXT("SoldierMotionReplicator"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));



	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	NearObjectCollisionDetector = CreateDefaultSubobject<USphereComponent>(TEXT("NearObjectCollisionDetector"));
	NearObjectCollisionDetector->SetupAttachment(RootComponent);


	

}


void APuzzlePlatformsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetInTheCar", IE_Pressed, this, &APuzzlePlatformsCharacter::GetInTheCar);

	PlayerInputComponent->BindAxis("MoveForward", this, &APuzzlePlatformsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APuzzlePlatformsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APuzzlePlatformsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APuzzlePlatformsCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APuzzlePlatformsCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APuzzlePlatformsCharacter::TouchStopped);

	// VR headset functionality
	
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APuzzlePlatformsCharacter::Attack);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APuzzlePlatformsCharacter::OnResetVR);
}


void APuzzlePlatformsCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//MyAnim =GetMesh()->GetAnimInstance();
	//ABCHECK(nullptr != MyAnim);

	//MyAnim->OnMontageEnded.AddDynamic(this, &APuzzlePlatformsCharacter::OnAttackMontageEnded);
	//MyAnim->OnAttackHitCheck.AddUObject(this, &APuzzlePlatformsCharacter::AttackCheck);

}

void APuzzlePlatformsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Not Working
	//FName identifier = TEXT("Not Yet");
	auto gamemode = Cast<AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsLocallyControlled()&& gamemode != nullptr)
	{
		//FText Name = Cast<AMyPlayerController>(GetController())->HUDWidget->PlayerName->GetText();
		//if(Name.EqualTo(FText::FromName(identifier)))

		UE_LOG(LogTemp, Warning, TEXT("LoadSetNameMenu"));
		Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->LoadSetNameMenu();
	}
}

void APuzzlePlatformsCharacter::Tick(float DeltaTime)
{//시작하자마자 로그인되는거임;;ㅋㅋ
	Super::Tick(DeltaTime);
	DrawDebugString(GetWorld(), FVector(0, 0, 150), GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);


}


void APuzzlePlatformsCharacter::GetInTheCar()
{
	ABCHECK(DaerimMotionReplicator !=nullptr)
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float CarRange = 100.f;
	float CarRadius = 50.f;


	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * CarRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(CarRadius),
		Params);
	FVector Vec = GetActorForwardVector() * CarRange;
	FVector Center = GetActorLocation() + CarRadius * 0.5f;
	float HalfHeight = CarRange * 0.5f + CarRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;


	DrawDebugCapsule(GetWorld(), Center, HalfHeight, CarRadius,
		Rotation, DrawColor, false, 5.f);
	if (bResult && HitResult.Actor.IsValid())
	{

		auto Car = Cast<AMyProjectPawn>(HitResult.Actor);

		if (Car != nullptr)
		{
			//DisableActor(true);
			//OtherActor->SetHidden(true);
			//GetController()->Possess(Car);

			//Car->SetRider(this);
			//Car->OurMovementComponent__->ItsMe = false;
			//Car->OurMovementComponent__->riden = true;
			DaerimMotionReplicator->Server_SendRide(Car, this);
		}
	}
}

void APuzzlePlatformsCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APuzzlePlatformsCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APuzzlePlatformsCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APuzzlePlatformsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APuzzlePlatformsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APuzzlePlatformsCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APuzzlePlatformsCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APuzzlePlatformsCharacter::CJump(float Value)
{
	Jump();
}
void APuzzlePlatformsCharacter::CStopJumping(float Value)
{
	StopJumping();
}

void APuzzlePlatformsCharacter::Attack()
{
	//만약 종족이 두개있다면..?
	UE_LOG(LogTemp, Warning, TEXT("Attack1"));
	if(DaerimMotionReplicator != nullptr)
		DaerimMotionReplicator->Server_SendAttack();

}




//void APuzzlePlatformsCharacter::OnRep_TakeDamage()
//{
//	FDamageEvent DamageEvent;
//	TakeDamage(50.0f, DamageEvent, GetController(), this);
//}


float APuzzlePlatformsCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{

	//ABCHECK(MotionReplicator != nullptr)
	
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	float HP =  CharacterStat->GetHP();
	CharacterStat->SetHP(HP - FinalDamage);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	return FinalDamage;


}

