// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Weapons/Weapon_Master.h"
#include "Weapons/FPSHudWidget.h"
#include "Missile//Missile.h"

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

#include "Net/UnrealNetwork.h"

ASoldier::ASoldier()
{
	bReplicates = true;
#pragma region AssetSettings
	//Asset Settings
	static ConstructorHelpers::FClassFinder<AMissile> MissileBPClass((TEXT("/Game/RocketPath/BP_Missile")));
	if (MissileBPClass.Succeeded())	MissileClass = MissileBPClass.Class;
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
	DaerimMotionReplicator = CreateDefaultSubobject<USoldierMotionReplicator>(TEXT("SoldierMotionReplicator"));
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
}



void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &APuzzlePlatformsCharacter::MoveForward);
	PlayerInputComponent->BindAction("WeaponPrimary", IE_Released, this, &ASoldier::WeaponPrimaryReleased);
	PlayerInputComponent->BindAction("WeaponSecondary", IE_Pressed, this, &ASoldier::WeaponSecondaryPressed);
	PlayerInputComponent->BindAction("WeaponSecondary", IE_Released, this, &ASoldier::WeaponSecondaryReleased);
	PlayerInputComponent->BindAction("WeaponReload", IE_Pressed, this, &ASoldier::WeaponReload);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASoldier::InteractPressed);

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

	if (EquippedItem == nullptr)
	{
		IsItemEquipped = false;
	}
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	
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

	if (IsAiming)
	{
		AimDownSights();
	}
	else
		UnAim();
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

	Server_SetMuzzleRotation(temp);
	Everyone_SetMuzzleRotation(temp);
}

void ASoldier::Everyone_SetMuzzleRotation_Implementation(FRotator NewRotator)
{

	EquippedItem->SetMuzzleRotation(NewRotator);
	
}
bool ASoldier::Everyone_SetMuzzleRotation_Validate(FRotator NewRotator)
{
	return true;
}

bool ASoldier::Server_SetMuzzleRotation_Validate(FRotator NewRotator)
{
	return true;
}

void ASoldier::Server_SetMuzzleRotation_Implementation(FRotator NewRotator)
{

	EquippedItem->SetMuzzleRotation(NewRotator);

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


void ASoldier::Attack()
{
	if (EquippedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippedItem null"));
		return;
	}
	SetMuzzleRotation();
	Super::Attack();

}

void ASoldier::WeaponPrimaryReleased()
{

	if (EquippedItem == nullptr)
		return;
	Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->Server_SendAttackStop();

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
void ASoldier::WeaponReload()
{
	if (EquippedItem == nullptr)
		return;
	bool IsFiring__ = Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->IsFiring;
	if (EquippedItem->CanReload == true && IsFiring__ == false && IsReloading == false && IsItemEquipped == true)
	{
		CanAim = false;
		IsReloading = true;

		FTimerHandle WaitHandle;
		EquippedItem->Reload();
		float WaitTime =EquippedItem->ReloadDelay; //시간을 설정하고
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{

				// 여기에 코드를 치면 된다.
				IsReloading = false;
				CanAim = true;
			}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능


	}
}

void ASoldier::EquipItem(AObject_Master* Item, bool EquipAndHold)
{
	if (EquipAndHold == false)
	{
		auto weapon = Cast<AWeapon_Master>(Item);
		if (weapon != nullptr)
		{
			EquippedItem = weapon;
			PrimaryWeapon = weapon;
			EquippedItem->Player = this;
			EquippedItem->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EquippedItem->AttachToPlayer(this ,"GripPoint");
			IsItemEquipped = true;
			bUseControllerRotationYaw= true;
			//자연스럽게 원하는 방향으로 회전
			GetCharacterMovement()->bOrientRotationToMovement = false;
			// 자동적으로 캐릭터의 이동방향을 움직이는 방향에 맞춰주며 회전보간을 해줌

			WeaponSlotUse = EWeaponSlot::TE_PrimaryWeapon;
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
	if (IsItemEquipped == false)
	{
		Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->Server_SendGetItem(PickupItem);
		SetFPSHudWidget();
	}
	else
		Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->Server_SendGetItem(PickupItem);

}



