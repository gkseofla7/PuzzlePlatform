// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character_Master.h"

#include"PlayersComponent/MyCharacterStatComponent.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "Cars/MyProjectPawn.h"
#include "PuzzlePlatformsGameInstance.h"
#include "MyPlayerController.h"
#include "UI/PlayerInfoWidget.h"
#include "PlayersComponent/SoldierMotionReplicator.h"
#include "MyLobbyGameMode.h"
#include "AbilitySystem/Ability.h"
#include "AbilitySystem/UI/ActionBarSlotWidget.h"
#include "AbilitySystem/UI/HudUpDisplayWidget.h"
#include "AbilitySystem/UI/ActionBarWidget.h"
#include "AbilitySystem/UI/CastBarWidget.h"
#include "AbilitySystem/ActorAbilities.h"
#include "UI/PlayerHPBarWidget.h"
#include "MyPlayerState.h"
#include "PlayersComponent/PointOfInterestComponent.h"

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
#include "Components/WidgetComponent.h"


#include "ImageUtils.h"


#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// ACharacter_Master


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
ACharacter_Master::ACharacter_Master()
{
#pragma region GeneralInitialize
	bReplicates = true;
	SetActorTickEnabled(true);
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	
#pragma endregion Region_1
	//Component 초기화
	ActorAbilitiesComponent = CreateDefaultSubobject<UActorAbilities>(TEXT("ActorAbilities"));

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetVisibility(false);

	NearObjectCollisionDetector = CreateDefaultSubobject<USphereComponent>(TEXT("NearObjectCollisionDetector"));
	NearObjectCollisionDetector->SetupAttachment(RootComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	HPBarWidget->SetupAttachment(GetMesh());

	PointOfInterestComponent = CreateDefaultSubobject<UPointOfInterestComponent>(TEXT("PointOfInterestComponent"));
	

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/PuzzlePlatforms/Widget/WBP_PlayerHPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}
	

}

void ACharacter_Master::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacter_Master, Level);
}


void ACharacter_Master::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &ACharacter_Master::Skill1Clicked);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &ACharacter_Master::Skill2Clicked);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &ACharacter_Master::Skill3Clicked);
	PlayerInputComponent->BindAction("Skill4", IE_Pressed, this, &ACharacter_Master::Skill4Clicked);
	PlayerInputComponent->BindAction("Skill5", IE_Pressed, this, &ACharacter_Master::Skill5Clicked);
	PlayerInputComponent->BindAction("Skill1", IE_Released, this, &ACharacter_Master::SkillReleased);
	PlayerInputComponent->BindAction("Skill2", IE_Released, this, &ACharacter_Master::SkillReleased);
	PlayerInputComponent->BindAction("Skill3", IE_Released, this, &ACharacter_Master::SkillReleased);
	PlayerInputComponent->BindAction("Skill4", IE_Released, this, &ACharacter_Master::SkillReleased);
	PlayerInputComponent->BindAction("Skill5", IE_Released, this, &ACharacter_Master::SkillReleased);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetInTheCar", IE_Pressed, this, &ACharacter_Master::GetInTheCar);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacter_Master::Attack);

	PlayerInputComponent->BindAction("SkillTree", IE_Pressed, this, &ACharacter_Master::OpenSkillTree);
	PlayerInputComponent->BindAction("OpenMap", IE_Pressed, this, &ACharacter_Master::OpenMap);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacter_Master::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacter_Master::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter_Master::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacter_Master::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter_Master::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacter_Master::LookUpAtRate);


}






void ACharacter_Master::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ACharacter_Master::PossessedBy(AController* NewController)//이것도 결국 서버에서 실행함
{//입장하면 자기 자신의 Level을 다른애들한테도 뿌림
	Super::PossessedBy(NewController);
	auto MyController = Cast<AMyPlayerController>(NewController);
	//Server_SetPlayerStat();
}

void ACharacter_Master::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())//체력 주기적으로 회복
	{
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ACharacter_Master::UpdateStat, 7, true);
	}
	auto LobbyGameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode != nullptr)//Lobby에서 안보이게 하려고
	{
		auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());
		CharacterWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ACharacter_Master::SetPlayerStat);//어차피 자기 자신만 실행함
	GetWorldTimerManager().SetTimer(StatResetHandle, RespawnDelegate, 2.f, false);

	//SetPlayerStat();
}

void ACharacter_Master::Server_SetPlayerStat_Implementation()
{
	NetMulticast_SetPlayerStat();
}

bool ACharacter_Master::Server_SetPlayerStat_Validate()
{
	return true;
}

void ACharacter_Master::NetMulticast_SetPlayerStat_Implementation()
{
	SetPlayerStat();
}

bool ACharacter_Master::NetMulticast_SetPlayerStat_Validate()
{
	return true;
}

void ACharacter_Master::SetPlayerStat()
{

	auto MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (MyPlayerState == nullptr)
		return;
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server : SetPlayerStat"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client : SetPlayerStat"));
	}
	CharacterStatRef = MyPlayerState->CharacterStat;
	if (HasAuthority())
	{
		CharacterStatRef->Respawn();
	}
	CharacterStatRef->CharacterRef = this;
	if (IsLocallyControlled() && IsPlayerControlled())//새로입장 or 리스폰 모두에게 내 정보 뿌려줌	
	{
		auto MyController = Cast<AMyPlayerController>(GetController());
		MyController->SetWidget(MyPlayerState->CharacterStat);//내 mainwidget
		PlayerInfoHUDWidget = MyController->PlayerInfoHUDWidget;
		Server_BindCharacterStatToWidget();//각 플레이어 머리 위 widget

	}
	else// 나 외 다른 플레이어들은
	{
		BindCharacterStatToWidget();
	}

	
}


void ACharacter_Master::Server_BindCharacterStatToWidget_Implementation()
{
	NetMulticast_BindCharacterStatToWidget();
}

bool ACharacter_Master::Server_BindCharacterStatToWidget_Validate()
{
	return true;
}
void  ACharacter_Master::NetMulticast_BindCharacterStatToWidget_Implementation()
{
	BindCharacterStatToWidget();
}

bool  ACharacter_Master::NetMulticast_BindCharacterStatToWidget_Validate()
{
	return true;
}

void ACharacter_Master::BindCharacterStatToWidget()
{
	auto MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());

	CharacterWidget->BindCharacterStat(MyPlayerState->CharacterStat);
	CharacterWidget->SetNameText(FText::FromString(MyPlayerState->GetPlayerName()));
}


void ACharacter_Master::Tick(float DeltaTime)
{//시작하자마자 로그인되는거임;;ㅋㅋ
	Super::Tick(DeltaTime);
	if(PlayerInfoHUDWidget !=nullptr)
		SkillAvailable = !(PlayerInfoHUDWidget->CastBar_UI->WhileBuffering);
	if (IsLocallyControlled())
	{
		//SetTargetPlayerWithLineTrace();
	}
	if (!IsLocallyControlled())
	{
		auto MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ABCHECK(MyController != nullptr);
		auto MyPawn = Cast< ACharacter_Master>(MyController->GetPawn());
		if (MyPawn == nullptr)
			return;
		//ABCHECK(MyPawn != nullptr);
		ABCHECK(HPBarWidget!=nullptr)
		auto Dir = MyPawn->FollowCamera->GetComponentLocation() - HPBarWidget->GetComponentLocation();
		auto DirRot = UKismetMathLibrary::MakeRotFromX(Dir);
		HPBarWidget->SetWorldRotation(DirRot);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetController()->GetName()));

		//아 자기가 0번째니깐!!
	}
	//HPBarWidget->SetWorldRotation(FVector(0.f, 0.f, 180.f));
}

void ACharacter_Master::AddControllerPitchInput(float Val)
{
	if (IsDashing == false)
	{
		Super::AddControllerPitchInput(Val);
	}
}
void ACharacter_Master::AddControllerYawInput(float Val)
{
	if (IsDashing == false)
	{
		Super::AddControllerYawInput(Val);
	}
}
void ACharacter_Master::UpdateStat()
{
	if (CharacterStatRef == nullptr)
		return;
	if (CharacterStatRef->CurrentHP < CharacterStatRef->CurrentStatData->MaxHP)
		CharacterStatRef->Server_SetHP(CharacterStatRef->CurrentHP + .5);
	if (CharacterStatRef->CurrentHP < CharacterStatRef->CurrentStatData->MaxMP)
		CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP+.5);
}

void ACharacter_Master::SetTargetPlayerWithLineTrace()
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
		auto tmp = Cast< ACharacter_Master>(Hit.Actor);
		if (TargetPlayer != nullptr && tmp!= nullptr)
		{
			TargetPlayer->DecalComponent->SetVisibility(false);
		}
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
		if (tmp != nullptr)
		{
			TargetPlayer = tmp;
			ReplicateComponent->Server_SetTargetPlayer(tmp);
		}
			if (TargetPlayer != nullptr)
		{
			//이후
			TargetPlayer->DecalComponent->SetVisibility(true);
		}
	}
	//FRotator temp = UKismetMathLibrary::FindLookAtRotation(Start, EndTrace);
}

void ACharacter_Master::TurnAtRate(float Rate)
{
	if (IsDashing == true)
		return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacter_Master::LookUpAtRate(float Rate)
{
	if (IsDashing == true)
		return;
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacter_Master::MoveForward(float Value)
{
	if (UsingSkill == true)
	{

		return;
	}
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

void ACharacter_Master::MoveRight(float Value)
{
	if (UsingSkill == true)
		return;
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

void ACharacter_Master::Attack()
{
	//만약 종족이 두개있다면..?

	if (IsAttacking == true || UsingSkill == true)
		return;
	if(ReplicateComponent != nullptr)
		ReplicateComponent->Server_SendAttack();

}

float ACharacter_Master::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
		return 0;
	if (bDead == true)
		return 0;
	//ABCHECK(MotionReplicator != nullptr)
	
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ABCHECK(CharacterStatRef != nullptr,0.f);

	float HP = CharacterStatRef->GetHP();

	CharacterStatRef->Server_SetHP(HP - FinalDamage);

	return FinalDamage;

}


FRotator ACharacter_Master::GetMuzzleRotation()
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

void ACharacter_Master::Skill1Clicked()
{
	if (SkillAvailable == false)
		return;
	auto Slot_UI = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable==false||Slot_UI->IsManaAvailable ==false )
		return;
	if (UsingSkill == true)
		return;
	auto SlotClass = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP-SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI->StartCooldown();
	ReplicateComponent->Server_Skill1Clicked(SlotClass);
}
void ACharacter_Master::Skill2Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_1;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_1->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_1->StartCooldown();
	ReplicateComponent->Server_Skill2Clicked(SlotClass);
}
void ACharacter_Master::Skill3Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_2;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_2->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_2->StartCooldown();
	ReplicateComponent->Server_Skill3Clicked(SlotClass);
}
void ACharacter_Master::Skill4Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_3;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_3->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);

	PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_3->StartCooldown();
	ReplicateComponent->Server_Skill4Clicked(SlotClass);
}
void ACharacter_Master::Skill5Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_4;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_4->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStatRef->Server_SetMP(CharacterStatRef->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	PlayerInfoHUDWidget->ActionBar_UI->ActionBarSlot_UI_4->StartCooldown();
	ReplicateComponent->Server_Skill5Clicked(SlotClass);
}

void ACharacter_Master::SkillReleased()
{
	OnSkillReleased.Broadcast();
}

void ACharacter_Master::SetIsAttacking(bool NewIsAttacking)
{
	ReplicateComponent->Server_SetIsAttacking(NewIsAttacking);
}

void ACharacter_Master::SetUsingSkill(bool NewUsingSkill)
{

	ReplicateComponent->Server_SetUsingSkill(NewUsingSkill);
}


void ACharacter_Master::Die()
{

}

void ACharacter_Master::DestroyPlayer()
{
	Destroy();
}

void ACharacter_Master::OpenSkillTree()
{
	auto controller = Cast<AMyPlayerController>(GetController());
	ABCHECK(controller != nullptr);
	ABCHECK(PlayerInfoHUDWidget != nullptr);
	if (MouseCursorToggle == false)
	{
		PlayerInfoHUDWidget->ToggleSpellBook();
		controller->SetInputModeGameAndUI();
		MouseCursorToggle = true;
	}
	else
	{
		PlayerInfoHUDWidget->ToggleSpellBook();
		controller->SetInputModeGame();
		MouseCursorToggle = false;
	}
}

void ACharacter_Master::OpenMap()
{
	auto controller = Cast<AMyPlayerController>(GetController());
	auto PlayerWidget = controller->PlayerInfoHUDWidget;
	ABCHECK(PlayerWidget != nullptr);

	PlayerWidget->ToggleMap();
	

}


void ACharacter_Master::GetInTheCar()
{
	ABCHECK(ReplicateComponent != nullptr)
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
			ReplicateComponent->Server_SendRide(Car, this);
		}
	}
}



//void ACharacter_Master::SeeMouseCursur()
//{
//	auto controller = Cast<AMyPlayerController>(GetController());
//	if (MouseCursorToggle == false)
//	{
//		HeadsUpDisplayRef->ToggleSpellBook();
//		controller->SetInputModeGameAndUI();
//
//		MouseCursorToggle = true;
//	}
//	else
//	{
//		HeadsUpDisplayRef->ToggleSpellBook();
//		controller->SetInputModeGame();
//		MouseCursorToggle = false;
//	}
//}