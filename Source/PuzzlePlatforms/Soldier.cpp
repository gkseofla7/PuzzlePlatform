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
	ADSCam_ = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCam"));
	ADSCam_->SetupAttachment(GetMesh());
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

void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetMuzzleRotation();

	if (IsAiming)
	{
		AimDownSights();
	}
	else
		UnAim();
}



void ASoldier::SetMuzzleRotation()
{
	FVector Start = EquippedItem->GetSkeletalMesh()->GetSocketLocation("Muzzle");
	FVector Target = AimObejctFPP->GetComponentLocation();
	EquippedItem->MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

}

void ASoldier::AimDownSights()
{
	FPPCam_->Deactivate();
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
	FVector VTarget = FPPCam_->GetComponentLocation();

	FRotator RCurrent = ADSCam_->GetComponentRotation();
	FRotator RTarget = FPPCam_ ->GetComponentRotation();

	float Deltatime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	FVector NextLoc = FMath::VInterpTo(VCurrent, VTarget, Deltatime, 15);
	FRotator NexRot = FMath::RInterpTo(RCurrent, RTarget, Deltatime, 15);

	ADSCam_->SetWorldLocationAndRotation(NextLoc, NexRot);

	if (FPPCam_->GetComponentLocation().Equals(ADSCam_->GetComponentLocation(), 0.01))
	{
		ADSCam_->Deactivate();
		FPPCam_->Activate();
	}
}

void ASoldier::WeaponPrimaryPressed()
{
	SetMuzzleRotation();
	EquippedItem->StartFire();
	IsFiring = true;
}

void ASoldier::WeaponPrimaryReleased()
{

	EquippedItem->StopFire();
	IsFiring = false;
}

void ASoldier::WeaponSecondaryPressed()
{
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
	if (EquippedItem->CanReload == true && IsFiring == false && IsReloading == false && IsItemEquipped == true)
	{
		IsReloading = true;
		EquippedItem->Reload();
		FTimerHandle WaitHandle;
		float WaitTime =EquippedItem->ReloadDelay; //시간을 설정하고
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				// 여기에 코드를 치면 된다.
				IsReloading = false;
			}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
	}
}
