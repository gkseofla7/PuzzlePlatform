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
#include "AbilitySystem/ActionBarSlotWidget.h"
#include "AbilitySystem/HudUpDisplayWidget.h"
#include "AbilitySystem/ActionBarWidget.h"
#include "AbilitySystem/CastBarWidget.h"

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
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"

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

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetVisibility(false);
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
	PlayerInputComponent->BindAction("SkillTree", IE_Pressed, this, &APuzzlePlatformsCharacter::SeeMouseCursur);
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill1Clicked);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill2Clicked);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill3Clicked);
	PlayerInputComponent->BindAction("Skill4", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill4Clicked);
	PlayerInputComponent->BindAction("Skill5", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill5Clicked);

	PlayerInputComponent->BindAxis("MoveForward", this, &APuzzlePlatformsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APuzzlePlatformsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APuzzlePlatformsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APuzzlePlatformsCharacter::LookUpAtRate);


	// VR headset functionality
	
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APuzzlePlatformsCharacter::Attack);

}


void APuzzlePlatformsCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APuzzlePlatformsCharacter::BeginPlay()
{
	Super::BeginPlay();
	HeadsUpDisplayRef = Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->HeadsUpDisplay;
	//Not Working
	//FName identifier = TEXT("Not Yet");
	auto gamemode = Cast<AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsLocallyControlled()&& gamemode != nullptr)
	{
		Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->LoadSetNameMenu();
	}
}

void APuzzlePlatformsCharacter::Tick(float DeltaTime)
{//시작하자마자 로그인되는거임;;ㅋㅋ
	Super::Tick(DeltaTime);
	SkillAvailable = !(HeadsUpDisplayRef->CastBar_UI->WhileBuffering);
	SetTargetPlayerWithLineTrace();

}

void APuzzlePlatformsCharacter::SetTargetPlayerWithLineTrace()
{
	UCameraComponent* CurrentCam = FollowCamera;

	const float SkillRange = 20000.f;
	const FVector StartTrace = CurrentCam->GetComponentLocation();
	FVector EndTrace = (CurrentCam->GetForwardVector() * SkillRange) + StartTrace;
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		//이전꺼
		auto tmp = Cast< APuzzlePlatformsCharacter>(Hit.Actor);
		if (TargetPlayer != nullptr && tmp!= nullptr)
		{
			TargetPlayer->DecalComponent->SetVisibility(false);
		}
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
		if(tmp != nullptr)
			TargetPlayer = tmp;
		if (TargetPlayer != nullptr)
		{
			//이후
			TargetPlayer->DecalComponent->SetVisibility(true);
		}
	}
	//FRotator temp = UKismetMathLibrary::FindLookAtRotation(Start, EndTrace);
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
			DaerimMotionReplicator->Server_SendRide(Car, this);
		}
	}
}


void APuzzlePlatformsCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APuzzlePlatformsCharacter::LookUpAtRate(float Rate)
{
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



void APuzzlePlatformsCharacter::Attack()
{
	//만약 종족이 두개있다면..?
	UE_LOG(LogTemp, Warning, TEXT("Attack1"));
	if(DaerimMotionReplicator != nullptr)
		DaerimMotionReplicator->Server_SendAttack();

}


float APuzzlePlatformsCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{

	//ABCHECK(MotionReplicator != nullptr)
	
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	float HP =  CharacterStat->GetHP();
	UE_LOG(LogTemp, Warning, TEXT("Before HP : %f"), CharacterStat->GetHP());
	CharacterStat->SetHP(HP - FinalDamage);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	UE_LOG(LogTemp, Warning, TEXT("After HP : %f"), CharacterStat->GetHP());
	return FinalDamage;


}


void APuzzlePlatformsCharacter::SeeMouseCursur()
{
	auto controller = Cast<AMyPlayerController>(GetController());
	if (MouseCursorToggle == false)
	{

		controller->SetInputModeGameAndUI();
		MouseCursorToggle = true;
	}
	else
	{
		controller->SetInputModeGame();
		MouseCursorToggle = false;
	}
}

FRotator APuzzlePlatformsCharacter::GetMuzzleRotation()
{

	UCameraComponent* CurrentCam = FollowCamera;

	const float WeaponRange = 20000.f;
	const FVector StartTrace = CurrentCam->GetComponentLocation();
	FVector EndTrace = (CurrentCam->GetForwardVector() * WeaponRange) + StartTrace;
	FVector Start = GetMesh()->GetSocketLocation("hand_rSocket");

	//FVector Target = AimObejctFPP->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
		EndTrace = Hit.ImpactPoint;

	}

	FRotator temp = UKismetMathLibrary::FindLookAtRotation(Start, EndTrace);

	return temp;
}

void APuzzlePlatformsCharacter::Skill1Clicked()
{
	if (SkillAvailable == false)
		return;
	Server_Skill1Clicked();
}

void APuzzlePlatformsCharacter::Skill2Clicked()
{
	if (SkillAvailable == false)
		return;
	Server_Skill2Clicked();
}

void APuzzlePlatformsCharacter::Skill3Clicked()
{
	if (SkillAvailable == false)
		return;
	Server_Skill3Clicked();
}

void APuzzlePlatformsCharacter::Skill4Clicked()
{
	if (SkillAvailable == false)
		return;
	Server_Skill4Clicked();
}
void APuzzlePlatformsCharacter::Skill5Clicked()
{
	if (SkillAvailable == false)
		return;
	if (TargetPlayer == nullptr)
		return;
	Server_Skill5Clicked();
}

void APuzzlePlatformsCharacter::Server_Skill1Clicked_Implementation()
{
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI->AbilitySpawn(this);

}

void APuzzlePlatformsCharacter::Server_Skill2Clicked_Implementation()
{
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_1->AbilitySpawn(this);
}


void APuzzlePlatformsCharacter::Server_Skill3Clicked_Implementation()
{
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_2->AbilitySpawn(this);
}

void APuzzlePlatformsCharacter::Server_Skill4Clicked_Implementation()
{
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_3->AbilitySpawn(this);
}
void APuzzlePlatformsCharacter::Server_Skill5Clicked_Implementation()
{
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_4->AbilitySpawn(this);
}

bool APuzzlePlatformsCharacter::Server_Skill1Clicked_Validate()
{
	return true;
}
bool APuzzlePlatformsCharacter::Server_Skill2Clicked_Validate()
{
	return true;
}
bool APuzzlePlatformsCharacter::Server_Skill3Clicked_Validate()
{
	return true;
}

bool APuzzlePlatformsCharacter::Server_Skill4Clicked_Validate()
{
	return true;
}

bool APuzzlePlatformsCharacter::Server_Skill5Clicked_Validate()
{
	return true;
}