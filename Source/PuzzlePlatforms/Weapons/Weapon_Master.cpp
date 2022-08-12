// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Master.h"
#include "BulletMaster.h"

#include "Kismet/KismetSystemLibrary.h"
AWeapon_Master::AWeapon_Master()
{
    ConstructorHelpers::FClassFinder<ABulletMaster> BulletBPClass(TEXT("/Game/Weapons/Projectiles/BP_Bullet_Master"));
    if (!ensure(BulletBPClass.Class != nullptr)) return;
    BulletMasterClass = BulletBPClass.Class;
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
    AmmoCheck();
    if ( CanFire== true && ClipEmpty == false && Reloading == false)
    {
       
        FVector BulletScale;
       // BulletScale.Set(0.1, 0.1, 0.1);
        FTransform BulletTransform;

        BulletTransform.SetLocation(SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation());
        BulletTransform.SetRotation(MuzzleRotation.Quaternion());
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