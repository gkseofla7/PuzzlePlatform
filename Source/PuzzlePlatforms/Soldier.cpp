// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Weapons/Weapon_Master.h"
#include "Weapons/FPSHudWidget.h"
#include "Missile//Missile.h"
#include "MyPlayerController.h"
#include "PuzzlePlatformsGameMode.h"

#include "Missile/TargetMarker.h"
#include "Missile/TargetableComponent.h"


#include "PlayersComponent/SoldierMotionReplicator.h"
#include "AnimInstance/SoldierAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Net/UnrealNetwork.h"

ASoldier::ASoldier()
{
	bReplicates = true;
#pragma region AssetSettings
	//Asset Settings
	static ConstructorHelpers::FClassFinder<AMissile> MissileBPClass((TEXT("/Game/RocketPath/BP_Missile")));
	if (MissileBPClass.Succeeded())	MissileClass = MissileBPClass.Class;

	static ConstructorHelpers::FClassFinder<ATargetMarker> TargetMarkerBPClass((TEXT("/Game/TargetMissiles/BP_TargetMarker")));
	if (TargetMarkerBPClass.Succeeded())	TargetMarkerClass = TargetMarkerBPClass.Class;
	//static ConstructorHelpers::FClassFinder<AMissile> GunBPClass((TEXT("/Game/Weapons/BP_Weapon_Master")));
	//if (GunBPClass.Succeeded())	GunClass = GunBPClass.Class;
	static ConstructorHelpers::FClassFinder<USoldierAnimInstance> SOLDIER_ANIM((TEXT("/Game/Animation/BP_SoldierAnim")));
	if (SOLDIER_ANIM.Succeeded())	GetMesh()->SetAnimInstanceClass(SOLDIER_ANIM.Class);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SplineStaticMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
	SplineStaticMesh = SplineStaticMeshAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>SplineStaticMaterialAsset(TEXT("/Game/RocketPath/M_Spline_White"));
	SplineStaticMaterial = SplineStaticMaterialAsset.Object;
	ConstructorHelpers::FClassFinder<AWeapon_Master> WeaponBPClass(TEXT("/Game/Weapons/BP_Weapon_Master"));
	if (!ensure(WeaponBPClass.Class != nullptr)) return;
	WeaponMasterClass = WeaponBPClass.Class;
#pragma endregion AssetSettings
	//ComponentSetting
#pragma region ComponentSetting
	ReplicateComponent = CreateDefaultSubobject<USoldierMotionReplicator>(TEXT("SoldierMotionReplicator"));
	RocketHolderComponent = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("RocketHolderComponent"));
	RocketHolderComponent->SetupAttachment(GetMesh(), "clavicle_rSocket");
	MissileComponent = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("MissileComponent"));
	MissileComponent->SetupAttachment(RocketHolderComponent, "Missile");
	GridSphere = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("GridSphere"));
	GridSphere->SetupAttachment(RootComponent);
	DecalMissileComponent = CreateDefaultSubobject< UDecalComponent>(TEXT("DecalMissileComponent"));
	DecalMissileComponent->SetupAttachment(GridSphere);	
	DecalMissileComponent->SetVisibility(false);
	SplinePathComponent = CreateDefaultSubobject< USplineComponent>(TEXT("SplinePathComponent"));
	SplinePathComponent->SetupAttachment(RocketHolderComponent);
	ADSCam_ = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCam"));
	ADSCam_->SetupAttachment(GetMesh());
#pragma endregion ComponentSetting
	//변수 초기화
	GeneralCameraPosition = FVector(0, 90, 90);
	MissileCameraPosition = FVector(0, 90, 200);
	CameraBoom->SetRelativeLocation(GeneralCameraPosition);
	IsItemEquipped = false;
	static ConstructorHelpers::FClassFinder<UUserWidget> FPSHudBPClass(TEXT("/Game/Weapons/UI/BP_FPS_Hud"));
	if (FPSHudBPClass.Succeeded())
	{
		FPSHudClass = FPSHudBPClass.Class;

	}
	TeamNum = 1;
}

void ASoldier::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASoldier, ControlRotation);

}

void ASoldier::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<USoldierAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);


}

void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast<UAnimInstance_Master>(GetMesh()->GetAnimInstance());
	FActorSpawnParameters SpawnParams;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	
}

void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacter_Master::MoveForward);
	PlayerInputComponent->BindAction("WeaponPrimary", IE_Released, this, &ASoldier::WeaponPrimaryReleased);
	PlayerInputComponent->BindAction("WeaponSecondary", IE_Pressed, this, &ASoldier::WeaponSecondaryPressed);
	PlayerInputComponent->BindAction("WeaponSecondary", IE_Released, this, &ASoldier::WeaponSecondaryReleased);
	PlayerInputComponent->BindAction("WeaponReload", IE_Pressed, this, &ASoldier::WeaponReload);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASoldier::InteractPressed);

}




void ASoldier::SetFPSHudWidget()
{
	if (FPSHudClass != nullptr)
	{
		HudWidget = CreateWidget<UFPSHudWidget>(GetWorld(), FPSHudClass);
		HudWidget->AddToViewport();

	}
}

void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetMuzzleRotation();
	if (EquippedItem!=nullptr &&IsLocallyControlled() && IsPlayerControlled())
	{
		ControlRotation = GetControlRotation();
		//float A = 360.0 - ControlRotation.Vector().X;
		float A = 360.0 - ControlRotation.Pitch;
		//float B = ControlRotation.Vector().Y* -1;
		float B = ControlRotation.Pitch;
		//float C = ControlRotation.Vector().Z;
		float C = ControlRotation.Yaw;
		float tmp = 0;
		if (B >= 180)
		{
			tmp = A / 3;
		}
		else
		{
			tmp = B * (-1) / 3;
		}
		ControlRotation = FRotator(0, 0, tmp);;
		Cast< USoldierMotionReplicator>(ReplicateComponent)->Server_SetControllRotation(ControlRotation);
	}
	if (IsAiming)
	{
		AimDownSights();
	}
	else
		UnAim();
	WearItem();
#pragma region SettingMissile
	if (ShowPath == true)
	{
		if (MissileTargetArmLength != CameraBoom->TargetArmLength)
			AimMissile();
		ClearPointsArray();

		auto RocketMouthTransform = RocketHolderComponent->GetSocketTransform("Mouth");
		auto ForwardVector = RocketMouthTransform.GetRotation().GetForwardVector();
		MissileVelocity = (ForwardVector + Direction * ForwardVector) * RocketSpeed;
	

		TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
		FPredictProjectilePathParams PredictParam;
		//FPredictProjectilePathResult PredictResult;
		TArray<AActor*> Ignores;
		Ignores.Add(this);
		PredictParam.StartLocation = RocketMouthTransform.GetLocation();
		PredictParam.LaunchVelocity = MissileVelocity;
		PredictParam.ProjectileRadius = 20.;
		PredictParam.bTraceWithCollision = true;
		PredictParam.ObjectTypes = ObjectTypes;
		PredictParam.SimFrequency = 15;
		//PredictParam.DrawDebugType = EDrawDebugTrace::ForDuration;
		//PredictParam.DrawDebugTime = .1;
		PredictParam.MaxSimTime = 10;
		PredictParam.ActorsToIgnore = Ignores;
		PredictParam.OverrideGravityZ = 0.;
		//결국 시작값, 이동등을 정해서 위치 경로 예측
		FHitResult OutHit;
		TArray< FVector > OutPathPositions;
		FVector OutLastTraceDestination;
		UGameplayStatics::PredictProjectilePath(GetWorld(), OutHit, OutPathPositions, OutLastTraceDestination, RocketMouthTransform.GetLocation(), MissileVelocity,
			true, 20., ObjectTypes, false, Ignores, EDrawDebugTrace::None, 0, 15, 10, 0);
			
		//UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParam, PredictResult);
	
		for (int i = 0; i < OutPathPositions.Num(); i++)
		{
			SplinePathComponent->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::World);//예상경로등록
		}
		SplinePathComponent->SetSplinePointType(OutPathPositions.Num() - 1, ESplinePointType::CurveClamped);//예상경로 마지막 부분에 마지막부분이라고 알려줌

		for (int i = 0; i < SplinePathComponent->GetNumberOfSplinePoints() - 2; i++)
		{

			AddSplineMeshComponent(OutPathPositions[i], SplinePathComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World),
				OutPathPositions[i + 1], SplinePathComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World));
		}


		auto NextLocation = UKismetMathLibrary::VInterpTo(OutLastTraceDestination, GridSphere->GetComponentLocation(), DeltaTime, 10);
		GridSphere->SetWorldLocation(NextLocation);
	}
	else if (GeneralTargetArmLength != CameraBoom->TargetArmLength)
	{
		UnAimMissile();
	}



#pragma endregion SettingMissile

	if (IsLocallyControlled()&& ShowTarget == true)
	{
		
		auto Target = FindBestTarget();
		SetCurrentTarget(Target);
	}
	else if (IsLocallyControlled() && ShowTarget == false)
	{
		EndTarget();
	}

}



void ASoldier::OnRep_ControlRotation()//서버에서 진행 안하니..?
{
	auto Anim = Cast<USoldierAnimInstance>(MyAnim);
	Anim->ClientTimeBetweenLastUpdates = Anim->ClientTimeSinceUpdate;
	Anim->ClientTimeSinceUpdate = 0;

	Anim->StartControllerRotator = Anim->AimRotation;
}


void ASoldier::AddControllerPitchInput(float Val)
{
	if (IsDashing == false)
	{
		Super::AddControllerPitchInput(Val);
		if (ShowPath == true)
		{
			float NewDirection = 0;
			NewDirection = UKismetMathLibrary::FInterpTo(Direction, Direction - Val, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5);
			Direction = NewDirection;
		}
	}
}


void ASoldier::SetMuzzleRotation()
{
	if (EquippedItem == nullptr)
		return;
	UCameraComponent* CurrentCam = FollowCamera;


	const float WeaponRange = 20000.f;
	const FVector StartTrace = CurrentCam->GetComponentLocation();
	 FVector EndTrace = (CurrentCam->GetForwardVector() * WeaponRange) + StartTrace;
	FVector Start = EquippedItem->GetSkeletalMesh()->GetSocketLocation("Muzzle");
	//FVector Target = AimObejctFPP->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		EndTrace = Hit.ImpactPoint;
		
	}

	FRotator temp = UKismetMathLibrary::FindLookAtRotation(Start, EndTrace);

	auto MyReplicateComponent = Cast< USoldierMotionReplicator>(ReplicateComponent);
	MyReplicateComponent->Server_SetMuzzleRotation(temp);

}

void ASoldier::SteamPack()
{
	Sprint();
	if (EquippedItem == nullptr)
		return;

	//Movement->MaxAcceleration = GeneralAcceleration;
	EquippedItem->SteamPack = true;

}

void ASoldier::UnSteamPack()
{
	UnSprint();
	if (EquippedItem != nullptr)
		EquippedItem->SteamPack = false;

}

void ASoldier::Attack()
{
	if (EquippedItem == nullptr || CanAim == false)
	{
		return;
	}
	SetMuzzleRotation();
	Super::Attack();

}

void ASoldier::AimDownSights()
{
	FollowCamera->Deactivate();
	ADSCam_->Activate();
	FVector VCurrent = ADSCam_->GetComponentLocation();
	FVector VTarget = EquippedItem->GetCamera()->GetComponentLocation();

	FRotator RCurrent = ADSCam_->GetComponentRotation();
	FRotator RTarget = EquippedItem->GetCamera()->GetComponentRotation();

	float Deltatime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	FVector NextLoc = FMath::VInterpTo(VCurrent, VTarget, Deltatime, 15);
	FRotator NexRot = FMath::RInterpTo(RCurrent, RTarget, Deltatime, 15);

	ADSCam_->SetWorldLocationAndRotation(NextLoc, NexRot);

}

void ASoldier::UnAim()
{
	FVector VCurrent = ADSCam_->GetComponentLocation();
	FVector VTarget = FollowCamera->GetComponentLocation();

	FRotator RCurrent = ADSCam_->GetComponentRotation();
	FRotator RTarget = FollowCamera->GetComponentRotation();

	float Deltatime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	FVector NextLoc = FMath::VInterpTo(VCurrent, VTarget, Deltatime, 15);
	FRotator NexRot = FMath::RInterpTo(RCurrent, RTarget, Deltatime, 15);

	ADSCam_->SetWorldLocationAndRotation(NextLoc, NexRot);

	if (FollowCamera->GetComponentLocation().Equals(ADSCam_->GetComponentLocation(), 0.01))
	{
		ADSCam_->Deactivate();
		FollowCamera->Activate();
	}
}

void ASoldier::AimMissile()
{

	FVector VCurrent = CameraBoom->GetRelativeLocation();
	FVector VTarget = MissileCameraPosition;

	float FCurrent = CameraBoom->TargetArmLength;
	float FTarget = MissileTargetArmLength;

	float Deltatime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	FVector NextLoc = FMath::VInterpTo(VCurrent, VTarget, Deltatime, 15);
	CameraBoom->SetRelativeLocation(NextLoc);

	float NextLenth = FMath::FInterpTo(FCurrent, FTarget, Deltatime, 15);
	CameraBoom->TargetArmLength = NextLenth;

}

void ASoldier::UnAimMissile()
{
	FVector VCurrent = CameraBoom->GetRelativeLocation();
	FVector VTarget = GeneralCameraPosition;

	float FCurrent = CameraBoom->TargetArmLength;
	float FTarget = GeneralTargetArmLength;

	float Deltatime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	FVector NextLoc = FMath::VInterpTo(VCurrent, VTarget, Deltatime, 15);
	CameraBoom->SetRelativeLocation(NextLoc);

	float NextLenth = FMath::FInterpTo(FCurrent, FTarget, Deltatime, 15);
	CameraBoom->TargetArmLength = NextLenth;

}


void ASoldier::WeaponPrimaryReleased()
{

	if (EquippedItem == nullptr)
		return;
	Cast<USoldierMotionReplicator>(ReplicateComponent)->Server_SendAttackStop();

}


void ASoldier::WeaponReload()
{
	if (EquippedItem == nullptr)
		return;
	bool MyIsFiring = Cast<USoldierMotionReplicator>(ReplicateComponent)->IsFiring;

	if (EquippedItem->CanReload == true && MyIsFiring == false && IsReloading == false && IsItemEquipped == true)
	{
		Cast<USoldierMotionReplicator>(ReplicateComponent)->Server_WeaponReload();
	}
}

void ASoldier::EquipItem(AObject_Master* Item, bool EquipAndHold)
{
	if (EquipAndHold == false)
	{
		auto weapon = Cast<AWeapon_Master>(Item);
		if (weapon != nullptr)
		{
			EquippedItem = weapon;//요건 전체
			PrimaryWeapon = weapon;//전체
			EquippedItem->Player = this;//전체
			EquippedItem->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EquippedItem->AttachToPlayer(this ,"GripPoint");//전체
			IsItemEquipped = true;//전체
			bUseControllerRotationYaw= true;
			//자연스럽게 원하는 방향으로 회전
			FTimerHandle WaitHandle;
			EquippedItem->Reload();
			float WaitTime = 1.73; //시간을 설정하고
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					CanAim = true;
				}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
			CanAim = true;//나중에 애니메이션 노티파이로변환 1.73
			GetCharacterMovement()->bOrientRotationToMovement = false;
			// 자동적으로 캐릭터의 이동방향을 움직이는 방향에 맞춰주며 회전보간을 해줌
			
			WeaponSlotUse = EWeaponSlot::TE_PrimaryWeapon;
			if (IsLocallyControlled()&&FPSHudClass != nullptr)
			{
				SetFPSHudWidget();

			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wiered Things happen"))
		auto weapon = Cast<AWeapon_Master>(Item);
		if (weapon != nullptr)
		{
			SecondaryWeapon = weapon;
			SecondaryWeapon->Player = this;
			SecondaryWeapon->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SecondaryWeapon->AttachToPlayer(this, "RifleHolster");
			
		}
	}
}




void ASoldier::InteractPressed()
{

	if (IsItemEquipped == false&& DoPickupLinetrace == true)
	{
		Cast<USoldierMotionReplicator>(ReplicateComponent)->Server_SendGetItem(PickupItem);
		//SetFPSHudWidget();
	}


}



void ASoldier::WearItem()//먹을게 있는지 찾아봄
{
	if (DoPickupLinetrace == true)
	{
		FVector StartLocation = FollowCamera->GetComponentLocation();
		FVector TargetLocation = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 600.;
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery9);
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutHits;
		//, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime
		UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, TargetLocation, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHits,true);
		if (OutHits.Num() > 0)
		{
			auto tmp = Cast<AWeapon_Master>(OutHits[0].GetActor());
			if (tmp != nullptr)
			{
				PickupItem = tmp;
			}
			else
			{
				PickupItem = nullptr;
			}
		}

	}
	else
		PickupItem = nullptr;
}

void ASoldier::Die()
{
	auto Anim = Cast<USoldierAnimInstance>(MyAnim);
	if (Anim != nullptr)
	{
		Anim->PlayDeathMontage();
		PlayersDied();
		//FTimerHandle TimerHandler;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASoldier::PlayersDied, 5, false);


	}
}

void ASoldier::PlayersDied()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	//GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	FTimerHandle VisibiltyTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(VisibiltyTimerHandler, this, &ACharacter_Master::UnvisiblePlayer, 2.5f, false);

	if (HasAuthority())
	{

		FTimerHandle DestroyTimerHandler;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandler, this, &ACharacter_Master::DestroyPlayer, 6, false);
	}
	if (IsLocallyControlled())
	{
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASoldier::RespawnCharacter, 5, false);
		
	}
}

void ASoldier::RespawnCharacter()//Run on Owning Client
{

	Cast<USoldierMotionReplicator>(ReplicateComponent)->Server_RespawnPawn(Cast<APlayerController>(GetController()));
	UnPossessed();
}




AActor* ASoldier::FindBestTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;//
	UClass* ActorClassFilter = AActor::StaticClass();
	TArray<AActor*> ActorsToIgnore;// TArray<AActor*>& OutActors)
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);//아마 이게 Character
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//아마 이게 Pawn
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), TargetingRange, ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);
	AActor* BestTarget =nullptr;
	float CurrentValue = 0;
	float BestValue = 1000 ;
	for (auto L_CurrentTarget : OutActors)
	{
		auto Comp = L_CurrentTarget->GetComponentByClass(UTargetableComponent::StaticClass());
		if (Comp != nullptr)
		{
			auto ForwardVector = GetActorForwardVector();
			auto DirectionToTarget = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), L_CurrentTarget->GetActorLocation());
			CurrentValue = FVector::DotProduct(ForwardVector, DirectionToTarget);
			if (BestValue > 100 || CurrentValue > BestValue)
			{
				BestTarget = L_CurrentTarget;
				BestValue = CurrentValue;
			}

		}
			
	}
	if (BestValue <100 && UKismetMathLibrary::DegAcos(BestValue) <= TargetingConeAngle)
	{
		return BestTarget;
	}
	else
	{
		return nullptr;
	}
}

void ASoldier::SetCurrentTarget(AActor* Target)
{
	if (CurrentTarget != Target)
	{
		EndTarget();
		CurrentTarget = Target;
		BeginTarget();

	}
}

void ASoldier::EndTarget()
{
	if (TargetMarker != nullptr)
	{
		TargetMarker->Destroy();
		TargetMarker = nullptr;
	}
}
void ASoldier::BeginTarget()
{
	if (CurrentTarget != nullptr)
	{
		CurrentTarget->GetActorLocation();
		FActorSpawnParameters ActorSpawnParameters;
		FTransform ActorTransform;
		ActorTransform.SetLocation(CurrentTarget->GetActorLocation());
		TargetMarker = GetWorld()->SpawnActor<ATargetMarker>(TargetMarkerClass.Get(), ActorTransform);
		TargetMarker->AttachToActor(CurrentTarget, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}



void ASoldier::WeaponSecondaryPressed()
{
	if (CanAim == false)
		return;
	if (EquippedItem != nullptr)
		IsAiming = true;

}
void ASoldier::WeaponSecondaryReleased()
{
	if (EquippedItem != nullptr)
		IsAiming = false;
}

//void ASoldier::Multicast_SetGun_Implementation(AWeapon_Master* weapon)
//{
//	EquippedItem = weapon;//요건 전체
//	PrimaryWeapon = weapon;//전체
//	EquippedItem->Player = this;//전체
//	EquippedItem->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	EquippedItem->AttachToPlayer(this, "GripPoint");//전체
//	IsItemEquipped = true;//전체
//	bUseControllerRotationYaw = true;
//	//자연스럽게 원하는 방향으로 회전
//	GetCharacterMovement()->bOrientRotationToMovement = false;
//	// 자동적으로 캐릭터의 이동방향을 움직이는 방향에 맞춰주며 회전보간을 해줌
//
//	WeaponSlotUse = EWeaponSlot::TE_PrimaryWeapon;
//}
//bool ASoldier::Multicast_SetGun_Validate(AWeapon_Master* NewItem)
//{
//	return true;
//}