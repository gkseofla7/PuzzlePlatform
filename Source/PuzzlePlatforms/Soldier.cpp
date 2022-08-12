// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Weapons/Weapon_Master.h"
#include "Weapons/FPSHudWidget.h"

#include "PlayersComponent/SoldierMotionReplicator.h"
#include "AnimInstance/SoldierAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
ASoldier::ASoldier()
{
	DaerimMotionReplicator = CreateDefaultSubobject<USoldierMotionReplicator>(TEXT("SoldierMotionReplicator"));

	if (DaerimMotionReplicator != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Here Is the problem"));
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
	FTransform WeaponTransform;
	EquippedItem = GetWorld()->SpawnActor<AWeapon_Master>(WeaponMasterClass, WeaponTransform);
	FAttachmentTransformRules ItemTransformRules(EAttachmentRule::SnapToTarget, true);
	//FAttachmentTransformRules ItemTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,  EAttachmentRule::SnapToTarget);


	EquippedItem->AttachToComponent(GetMesh(), ItemTransformRules,"GripPoint");


	//HudWidget = CreateWidget<UFPSHudWidget>(GetController(), FPSHudClass);
	//ABCHECK(FPSHudClass != nullptr);


	if (FPSHudClass != nullptr)
	{

		HudWidget = CreateWidget<UFPSHudWidget>(GetWorld(), FPSHudClass);
		HudWidget->AddToViewport();

	}
	
}



void ASoldier::SetMuzzleRotation()
{
	FVector Start = EquippedItem->GetSkeletalMesh()->GetSocketLocation("Muzzle");
	FVector Target = AimObejctFPP->GetComponentLocation();
	EquippedItem->MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

}

void ASoldier::AimDownSights()
{
	
}