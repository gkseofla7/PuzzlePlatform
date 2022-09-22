// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsCharacter.h"

#include"PlayersComponent/MyCharacterStatComponent.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayersComponent/MotionReplicatorInterface.h"
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
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"


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
	
	MiniMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArm"));
	MiniMapSpringArm->SetupAttachment(RootComponent);
	MiniMapSpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//MiniMapSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(MiniMapSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//MiniMapCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
#pragma endregion Region_1
	//Component 초기화
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
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

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/PuzzlePlatforms/Widget/WBP_PlayerHPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}
	

}

void APuzzlePlatformsCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APuzzlePlatformsCharacter, Level);
}


void APuzzlePlatformsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("GetInTheCar", IE_Pressed, this, &APuzzlePlatformsCharacter::GetInTheCar);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APuzzlePlatformsCharacter::Attack);
	PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill1Clicked);
	PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill2Clicked);
	PlayerInputComponent->BindAction("Skill3", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill3Clicked);
	PlayerInputComponent->BindAction("Skill4", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill4Clicked);
	PlayerInputComponent->BindAction("Skill5", IE_Pressed, this, &APuzzlePlatformsCharacter::Skill5Clicked);
	PlayerInputComponent->BindAction("Skill1", IE_Released, this, &APuzzlePlatformsCharacter::SkillReleased);
	PlayerInputComponent->BindAction("Skill2", IE_Released, this, &APuzzlePlatformsCharacter::SkillReleased);
	PlayerInputComponent->BindAction("Skill3", IE_Released, this, &APuzzlePlatformsCharacter::SkillReleased);
	PlayerInputComponent->BindAction("Skill4", IE_Released, this, &APuzzlePlatformsCharacter::SkillReleased);
	PlayerInputComponent->BindAction("Skill5", IE_Released, this, &APuzzlePlatformsCharacter::SkillReleased);
	PlayerInputComponent->BindAction("SkillTree", IE_Pressed, this, &APuzzlePlatformsCharacter::OpenSkillTree);
	PlayerInputComponent->BindAction("OpenMap", IE_Pressed, this, &APuzzlePlatformsCharacter::OpenMap);
	PlayerInputComponent->BindAxis("MoveForward", this, &APuzzlePlatformsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APuzzlePlatformsCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APuzzlePlatformsCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APuzzlePlatformsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APuzzlePlatformsCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APuzzlePlatformsCharacter::LookUpAtRate);


}


UTextureRenderTarget2D* APuzzlePlatformsCharacter::CreateRenderTarget2D(int32 width, int32 height, bool makeHDR)
{
	UTextureRenderTarget2D* renderTarget = NewObject<UTextureRenderTarget2D>();

	if (makeHDR)
		renderTarget->InitAutoFormat(width, height);
	else
		renderTarget->InitCustomFormat(width, height, PF_B8G8R8A8, false);

	return renderTarget;
}

//bool APuzzlePlatformsCharacter::SaveRenderTarget(UTextureRenderTarget2D* renderTarget, FString path, FString fileName)
//{
//	FTextureRenderTargetResource* resource = renderTarget->GameThread_GetRenderTargetResource();
//	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);
//
//	TArray<FColor> outBMP;
//	outBMP.AddUninitialized(renderTarget->GetSurfaceWidth() * renderTarget->GetSurfaceHeight());
//	resource->ReadPixels(outBMP, readPixelFlags);
//
//	for (FColor& color : outBMP)
//	{
//		color.A = 255;
//	}
//
//	FIntPoint destSize(renderTarget->GetSurfaceWidth(), renderTarget->GetSurfaceHeight());
//	TArray<uint8> compressedBitmap;
//	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, compressedBitmap);
//
//	//FString fullPath = FPaths::Combine(path, fileName);
//
//	//bool imageSavedOK = FFileHelper::SaveArrayToFile(compressedBitmap, *fullPath);
//
//	return imageSavedOK;
//}

void APuzzlePlatformsCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APuzzlePlatformsCharacter::PossessedBy(AController* NewController)//이것도 결국 서버에서 실행함
{//입장하면 자기 자신의 Level을 다른애들한테도 뿌림
	Super::PossessedBy(NewController);
	auto MyController = Cast<AMyPlayerController>(NewController);

}

void APuzzlePlatformsCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())//체력 주기적으로 회복 아 애초에 이것도 실행을 안하는구나.. 누가 들어와도
	{
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &APuzzlePlatformsCharacter::UpdateStat, 7, true);
	}

	auto LobbyGameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode != nullptr)//Lobby에서 안보이게 하려고
	{
		auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());
		CharacterWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	//여기서 애들 Stat 초기화 시킴
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &APuzzlePlatformsCharacter::SetPlayerStat);//어차피 자기 자신만 실행함
	GetWorldTimerManager().SetTimer(StatResetHandle, RespawnDelegate, .3f, false);



}

void APuzzlePlatformsCharacter::SetPlayerStat()
{
	auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	auto tmpPlayerState = GetPlayerState();
	if (tmpPlayerState == nullptr)
		return;
	auto MyPlayerState = Cast<AMyPlayerState>(tmpPlayerState);
	if (MyPlayerState != nullptr)
	{
		//여기서 모두(중간에 들어오는 경우는 없으니)
		CharacterWidget->SetNameText(FText::FromString(MyPlayerState->GetPlayerName()));
		auto MyController = Cast<AMyPlayerController>(GetController());

		if (IsLocallyControlled() && IsPlayerControlled())//새로입장
		{
			//PlayerStat 초기화(다른애들 모두) 시키고 Widget에 바인딩
			MyPlayerState->Server_InitializeCharacterStat();//얘는 처음 입장한애만 Stat초기화시킴
			
			Server_BindCharacterStat();//모두에게 해당 Stat을 UI에 Binding
			//나만 바인딩하는애
			if (MyController->HasWidget == false)//Widget아직 안열려있으면 widget viewport함, 죽었다 살아났다해서
				MyController->SetWidget();
			MyController->BindWidget(CharacterStat);
			MyController->PlayerInfoHUDWidget->BindCharacterName(FText::FromString(MyPlayerState->GetPlayerName()));
		}
		if (!IsLocallyControlled())//클라이언트가 순차적으로 들어오니 문제발생해서
		{
			CharacterStat->SetLevel(MyPlayerState->PlayerLevel);
			CharacterWidget->BindCharacterStat(CharacterStat);
		}

	}
	
}


void APuzzlePlatformsCharacter::Server_BindCharacterStat_Implementation()
{
	NetMulticast_BindCharacterStat();
}

bool APuzzlePlatformsCharacter::Server_BindCharacterStat_Validate()
{
	return true;
}
void  APuzzlePlatformsCharacter::NetMulticast_BindCharacterStat_Implementation()
{
	auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	CharacterWidget->BindCharacterStat(CharacterStat);
}

bool  APuzzlePlatformsCharacter::NetMulticast_BindCharacterStat_Validate()
{
	return true;
}
//void APuzzlePlatformsCharacter::Test()
//{
//	if (GetController() != nullptr)
//	{
//		auto MyController = Cast<AMyPlayerController>(GetController());
//		ABCHECK(MyController !=nullptr);
//		if (HasAuthority() == true)
//		{
//
//			UE_LOG(LogTemp, Warning, TEXT("Server Character Beginplay %s  : %d"), *GetName(), MyController->test);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Client Character Beginplay %s  : %d"), *GetName(), Cast<AMyPlayerController>(GetController())->test);
//		}
//	}
//}


void APuzzlePlatformsCharacter::SetStatComponentLevel()
{

	if (Level == 0)
		return;
	else
	{
		CharacterStat->Level = Level;//뒤늦게 level 들어옴
		CharacterStat->LevelUp(Level);
		auto CharacterWidget = Cast< UPlayerHPBarWidget>(HPBarWidget->GetUserWidgetObject());
		if (nullptr != CharacterWidget)
		{
			CharacterWidget->BindCharacterStat(CharacterStat);
		}
		GetWorldTimerManager().ClearTimer(StatResetHandle);

	}
}

void APuzzlePlatformsCharacter::Tick(float DeltaTime)
{//시작하자마자 로그인되는거임;;ㅋㅋ
	Super::Tick(DeltaTime);
	if(HeadsUpDisplayRef!=nullptr)
		SkillAvailable = !(HeadsUpDisplayRef->CastBar_UI->WhileBuffering);
	if (IsLocallyControlled())
	{
		//SetTargetPlayerWithLineTrace();
	}
	if (!IsLocallyControlled())
	{
		auto MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ABCHECK(MyController != nullptr);
		auto MyPawn = Cast< APuzzlePlatformsCharacter>(MyController->GetPawn());
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

void APuzzlePlatformsCharacter::AddControllerPitchInput(float Val)
{
	if (IsDashing == false)
	{
		Super::AddControllerPitchInput(Val);
	}
}
void APuzzlePlatformsCharacter::AddControllerYawInput(float Val)
{
	if (IsDashing == false)
	{
		Super::AddControllerYawInput(Val);
	}
}
void APuzzlePlatformsCharacter::UpdateStat()
{
	if (CharacterStat == nullptr)
		return;
	CharacterStat->Server_SetHP(CharacterStat->CurrentHP+.5);
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP+.5);
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
		if (tmp != nullptr)
		{
			TargetPlayer = tmp;
			DaerimMotionReplicator->Server_SetTargetPlayer(tmp);
		}
			if (TargetPlayer != nullptr)
		{
			//이후
			TargetPlayer->DecalComponent->SetVisibility(true);
		}
	}
	//FRotator temp = UKismetMathLibrary::FindLookAtRotation(Start, EndTrace);
}

void APuzzlePlatformsCharacter::TurnAtRate(float Rate)
{
	if (IsDashing == true)
		return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APuzzlePlatformsCharacter::LookUpAtRate(float Rate)
{
	if (IsDashing == true)
		return;
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APuzzlePlatformsCharacter::MoveForward(float Value)
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

void APuzzlePlatformsCharacter::MoveRight(float Value)
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

void APuzzlePlatformsCharacter::Attack()
{
	//만약 종족이 두개있다면..?

	if (IsAttacking == true || UsingSkill == true)
		return;
	if(DaerimMotionReplicator != nullptr)
		DaerimMotionReplicator->Server_SendAttack();

}

float APuzzlePlatformsCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
		return 0;
	if (bDead == true)
		return 0;
	//ABCHECK(MotionReplicator != nullptr)
	
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	float HP =  CharacterStat->GetHP();

	CharacterStat->Server_SetHP(HP - FinalDamage);

	return FinalDamage;

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
	auto Slot_UI = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable==false||Slot_UI->IsManaAvailable ==false )
		return;
	if (UsingSkill == true)
		return;
	auto SlotClass = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP-SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI->StartCooldown();
	DaerimMotionReplicator->Server_Skill1Clicked(SlotClass);
}
void APuzzlePlatformsCharacter::Skill2Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_1;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_1->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_1->StartCooldown();
	DaerimMotionReplicator->Server_Skill2Clicked(SlotClass);
}
void APuzzlePlatformsCharacter::Skill3Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_2;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_2->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_2->StartCooldown();
	DaerimMotionReplicator->Server_Skill3Clicked(SlotClass);
}
void APuzzlePlatformsCharacter::Skill4Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_3;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_3->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);

	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_3->StartCooldown();
	DaerimMotionReplicator->Server_Skill4Clicked(SlotClass);
}
void APuzzlePlatformsCharacter::Skill5Clicked()
{
	if (SkillAvailable == false)
		return;

	auto Slot_UI = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_4;
	if (Slot_UI == nullptr)
		return;
	if (Slot_UI->IsAvailable == false || Slot_UI->IsManaAvailable == false)
		return;
	auto SlotClass = HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_4->AbilityClass;
	if (SlotClass == nullptr)
		return;
	CharacterStat->Server_SetMP(CharacterStat->CurrentMP - SlotClass.GetDefaultObject()->AbilityDetails.Cost);
	HeadsUpDisplayRef->ActionBar_UI->ActionBarSlot_UI_4->StartCooldown();
	DaerimMotionReplicator->Server_Skill5Clicked(SlotClass);
}

void APuzzlePlatformsCharacter::SkillReleased()
{
	OnSkillReleased.Broadcast();
}

void APuzzlePlatformsCharacter::SetIsAttacking(bool NewIsAttacking)
{
	DaerimMotionReplicator->Server_SetIsAttacking(NewIsAttacking);
}

void APuzzlePlatformsCharacter::SetUsingSkill(bool NewUsingSkill)
{

	DaerimMotionReplicator->Server_SetUsingSkill(NewUsingSkill);
}


void APuzzlePlatformsCharacter::Die()
{

}

void APuzzlePlatformsCharacter::DestroyPlayer()
{
	Destroy();
}





void APuzzlePlatformsCharacter::OpenSkillTree()
{
	auto controller = Cast<AMyPlayerController>(GetController());
	ABCHECK(controller != nullptr);
	ABCHECK(HeadsUpDisplayRef != nullptr);
	if (MouseCursorToggle == false)
	{
		HeadsUpDisplayRef->ToggleSpellBook();
		controller->SetInputModeGameAndUI();
		MouseCursorToggle = true;
	}
	else
	{
		HeadsUpDisplayRef->ToggleSpellBook();
		controller->SetInputModeGame();
		MouseCursorToggle = false;
	}
}

void APuzzlePlatformsCharacter::OpenMap()
{
	auto controller = Cast<AMyPlayerController>(GetController());
	auto PlayerWidget = controller->PlayerInfoHUDWidget;
	ABCHECK(PlayerWidget != nullptr);

	PlayerWidget->ToggleMap();
	

}


void APuzzlePlatformsCharacter::GetInTheCar()
{
	ABCHECK(DaerimMotionReplicator != nullptr)
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



//void APuzzlePlatformsCharacter::SeeMouseCursur()
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