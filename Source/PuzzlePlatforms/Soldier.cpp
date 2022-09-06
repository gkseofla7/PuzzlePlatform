// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Weapons/Weapon_Master.h"
#include "Weapons/FPSHudWidget.h"

#include "PlayersComponent/SoldierMotionReplicator.h"
#include "AnimInstance/SoldierAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Net/UnrealNetwork.h"

ASoldier::ASoldier()
{
	bReplicates = true;
	//SetReplicates(true);
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

	//SplineStaticMesh = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("SplineStaticMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SplineStaticMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
	SplineStaticMesh = SplineStaticMeshAsset.Object;


	static ConstructorHelpers::FObjectFinder<UMaterial>SplineStaticMaterialAsset(TEXT("/Game/RocketPath/M_Spline_White"));
	SplineStaticMaterial = SplineStaticMaterialAsset.Object;

	static ConstructorHelpers::FClassFinder<USoldierAnimInstance> SOLDIER_ANIM((TEXT("/Game/Animation/BP_SoldierAnim")));
	if (SOLDIER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SOLDIER_ANIM.Class);
	}

	ConstructorHelpers::FClassFinder<AWeapon_Master> WeaponBPClass(TEXT("/Game/Weapons/BP_Weapon_Master"));
	if (!ensure(WeaponBPClass.Class != nullptr)) return;
	WeaponMasterClass = WeaponBPClass.Class;

	FPPCam_ = CreateDefaultSubobject<UCameraComponent>(TEXT("FPPCam"));
	SpringArm_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	AimObejctFPP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimObejctFPP"));
	FPPCam_->SetupAttachment(GetMesh(),"head");
	SpringArm_->SetupAttachment(FPPCam_);
	AimObejctFPP->SetupAttachment(SpringArm_, USpringArmComponent::SocketName);
	ADSCam_ = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCam"));
	ADSCam_->SetupAttachment(GetMesh());
	IsItemEquipped = false;
}



void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings

	PlayerInputComponent->BindAction("WeaponPrimary", IE_Pressed, this, &ASoldier::WeaponPrimaryPressed);
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

	if (ShowPath == true)
	{
		
		//{


	//	//	PointsArray[0]->DestroyComponent();
	//	//
	//	//	PointsArray.Empty();
	//	//	SplinePathComponent->ClearSplinePoints();

	//	//}

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
		// 
		//UE_LOG(LogTemp, Warning, TEXT("PointsArray  Num: %d "), PointsArray.Num());
		//{
		//	FTransform RelativeTransform;
		//	auto SplineMesh = Cast< USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, RelativeTransform, true));

		//	FVector2D SplineScale(1000, 1000);
		//	SplineMesh->SetStartScale(SplineScale);
		//	SplineMesh->SetEndScale(SplineScale);
		//	SplineMesh->SetVisibility(true);
		//	SplineMesh->SetMobility(EComponentMobility::Movable);
		//	SplineMesh->SetStaticMesh(SplineStaticMesh);
		//	SplineMesh->SetWorldScale3D(FVector(100, 100, 100));
		//	SplineMesh->SetMaterial(0, SplineStaticMaterial);

		//	SplineMesh->ForwardAxis = ESplineMeshAxis::Z;
		//	PointsArray.Add(SplineMesh);
		//	SplineMesh->SetStartAndEnd(OutPathPositions[i], SplinePathComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World),
		//		OutPathPositions[i + 1], SplinePathComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World));
		//	SplineMesh->SetWorldLocation(OutPathPositions[i]);
		//	//	UE_LOG(LogTemp, Warning, TEXT("SplineMesh Location %f, %f, %fl "), OutPathPositions[i].X, OutPathPositions[i].Y, OutPathPositions[i].Z);
		//	//	UE_LOG(LogTemp, Warning, TEXT("SplineMesh Location %f, %f, %fl "),SplineMesh->GetStartPosition().X, SplineMesh->GetStartPosition().Y, SplineMesh->GetStartPosition().Z);
		//}

		auto NextLocation = UKismetMathLibrary::VInterpTo(OutLastTraceDestination, GridSphere->GetComponentLocation(), DeltaTime, 10);
		GridSphere->SetWorldLocation(NextLocation);
	}


}



void ASoldier::SetMuzzleRotation()
{
	if (EquippedItem == nullptr)
		return;
	UCameraComponent* CurrentCam;
	if (CamInUse == ECamInUse::TE_FPCam)
	{
		CurrentCam = FPPCam_;
	}
	else
	{
		CurrentCam = FollowCamera;
	}

	const float WeaponRange = 20000.f;
	const FVector StartTrace = CurrentCam->GetComponentLocation();
	 FVector EndTrace = (CurrentCam->GetForwardVector() * WeaponRange) + StartTrace;
	FVector Start = EquippedItem->GetSkeletalMesh()->GetSocketLocation("Muzzle");
	//FVector Target = AimObejctFPP->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	UE_LOG(LogTemp, Warning, TEXT("Before %f, %f, %f"), EndTrace.X, EndTrace.Y, EndTrace.Z);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("Got Line Trace"));
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
		EndTrace = Hit.ImpactPoint;
		UE_LOG(LogTemp, Warning, TEXT("After %f, %f, %f"), EndTrace.X, EndTrace.Y, EndTrace.Z);
		
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

void ASoldier::WeaponPrimaryPressed()
{

	//SetMuzzleRotation();
	//EquippedItem->StartFire();
	//Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->IsFiring = true;
	
	//IsFiring = true;
	//Cast<USoldierMotionReplicator>(DaerimMotionReplicator)->Server_SendAttack();
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
//UActorComponent* ASoldier::AddActorComponent(AActor* Owner, TSubclassOf<UActorComponent> ActorComponentClass)
//{
//	UClass* baseClass = FindObject<UClass>(ANY_PACKAGE, TEXT("ActorComponent"));
//	if (ActorComponentClass->IsChildOf(baseClass))
//	{
//		UActorComponent* NewComp = NewObject<UActorComponent>(Owner, ActorComponentClass);
//		if (!NewComp)
//		{
//			return NULL;
//		}
//		~~~~~~~~~~~~~
//
//		NewComp->RegisterComponent();        //You must ConstructObject with a valid Outer that has world, see above     
//
//		return NewComp;
//	}
//	else
//		return NULL;
//}