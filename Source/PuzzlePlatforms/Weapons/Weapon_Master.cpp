// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Master.h"
#include "BulletMaster.h"
#include "../Soldier.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
AWeapon_Master::AWeapon_Master()
{
    bReplicates = true;
    ConstructorHelpers::FClassFinder<ABulletMaster> BulletBPClass(TEXT("/Game/Weapons/Projectiles/BP_Bullet_Master"));
    if (!ensure(BulletBPClass.Class != nullptr)) return;
    BulletMasterClass = BulletBPClass.Class;

    Camera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera_->SetupAttachment(GetSkeletalMesh());
}

void AWeapon_Master::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AWeapon_Master, MuzzleRotation_);

}
void AWeapon_Master::Tick(float Deltatime)
{
    Super::Tick(Deltatime);
  //  DrawDebugString(GetWorld(), FVector(0, 0, 120), GetName(), this, FColor::White,Deltatime);

}

void AWeapon_Master::FireModeSwitch()
{
    switch (FireMode)
    {
    case EFireMode::TE_Single:
        FireMode = EFireMode::TE_Burst;
        break;
    case EFireMode::TE_Burst:
        FireMode = EFireMode::TE_Auto;
        break;
    case EFireMode::TE_Auto:
        FireMode = EFireMode::TE_Single;
        break;

    }



}

void AWeapon_Master::AmmoCheck()
{
    AmmoNeeded = MaxClipAmmo - ClipAmmo;
    if (AmmoNeeded <= BagAmmo)
    {
        HasEnoughAmmo = true;
        UseRemainingAmmo = false;
        CanReload = true;
    }
    else
    {
        if (BagAmmo <= 0)
        {
            HasEnoughAmmo = false;
            UseRemainingAmmo = false;
            CanReload = false;
        }
        else
        {
            HasEnoughAmmo = true;
            UseRemainingAmmo = true;
            CanReload = true;
        }
    }

    if (HasEnoughAmmo || UseRemainingAmmo)
    {
        if (ClipAmmo < MaxBagAmmo)
        {
            CanReload = true;
        }
        else
            CanReload = false;
    }
    else
        CanReload = false;

    ClipEmpty = (ClipAmmo <= 0);
}


void AWeapon_Master::Shot()
{
    FString name = GetName();
    UE_LOG(LogTemp, Warning, TEXT(" %s Muzzle Rotation %f, %f, %f"),*name, MuzzleRotation_.Yaw, MuzzleRotation_.Roll, MuzzleRotation_.Pitch);

    UE_LOG(LogTemp, Warning, TEXT("Shoot!!"));
    AmmoCheck();
    if (Soldier->IsLocallyControlled())
    {
        Soldier->SetMuzzleRotation();
    }

 
    if ( CanFire== true && ClipEmpty == false && Reloading == false)
    {
        
        FVector BulletScale;
       // BulletScale.Set(0.1, 0.1, 0.1);
        FTransform BulletTransform;

        BulletTransform.SetLocation(SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation());
        BulletTransform.SetRotation(MuzzleRotation_.Quaternion());
        //BulletTransform.SetScale3D(BulletScale);


        GetWorld()->SpawnActor<ABulletMaster>(BulletMasterClass, BulletTransform);

        ClipAmmo = ClipAmmo - AmmoCost;
    }
}

void AWeapon_Master::Reload()
{
    AmmoCheck();
    if (CanReload)
    {
        Reloading = true;


        //UKismetSystemLibrary::Delay(GetWorld(), ReloadDelay);

        FTimerHandle WaitHandle;
        float WaitTime = ReloadDelay; //시간을 설정하고
        GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
            {
                // 여기에 코드를 치면 된다.
                if (UseRemainingAmmo == true)
                {
                    ClipAmmo = ClipAmmo + BagAmmo;
                    BagAmmo = 0;
                    AmmoNeeded = 0;
                    Reloading = false;
                }
                else
                {
                    ClipAmmo = ClipAmmo + AmmoNeeded;
                    BagAmmo -=AmmoNeeded;
                    AmmoNeeded = 0;
                    Reloading = false;
                }
            }), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
    }
}

void AWeapon_Master::AttachToSoldier(ASoldier* NewSoldier ,FName SocketName )
{
    Soldier = NewSoldier;
    GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   
    FAttachmentTransformRules ItemTransformRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Soldier->GetMesh(), ItemTransformRules, SocketName);

}