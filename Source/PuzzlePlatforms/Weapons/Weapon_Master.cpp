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
//void AWeapon_Master::CustomInitialize(class ASoldier* NewPlayer)
//{
//    PlayerRef = NewPlayer;
//}

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
    Multicast_AmmoCheck();
}

void AWeapon_Master::Multicast_AmmoCheck_Implementation()
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

bool AWeapon_Master::Multicast_AmmoCheck_Validate()
{
    return true;
}

void AWeapon_Master::Shot()
{
    
    FString name = GetName();
    auto Soldier = Cast<ASoldier>(Player);
    AmmoCheck();

    if ( CanFire== true && ClipEmpty == false && Reloading == false)
    {
       Multicast_SetMuzzleRotation();//이걸 조종하는애 가져옴
        FVector BulletScale;
       // BulletScale.Set(0.1, 0.1, 0.1);
        FTransform BulletTransform;
        FVector Dir = GetOwner()->GetActorForwardVector();
        BulletTransform.SetLocation(SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation());
        BulletTransform.SetRotation(MuzzleRotation_.Quaternion());
        //BulletTransform.SetScale3D(BulletScale);


       ABulletMaster* bullet =  GetWorld()->SpawnActor<ABulletMaster>(BulletMasterClass, BulletTransform);
       if (bullet != nullptr)
       {
           bullet->Shooter = Soldier;
       }
       else
       {
           UE_LOG(LogTemp, Warning, TEXT("nullptr"));
       }
       Multicast_SetClipAmmo(ClipAmmo - AmmoCost);//모든 애들 총알 관리함~



        if (FireSound != NULL)//소리 다른애들한테도 해줘야됨
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }
        if (MuzzlesParticle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlesParticle, SkeletalMeshComponent->GetSocketTransform(FName("Muzzle")));
        }
       

    }
}

void AWeapon_Master::Multicast_SetClipAmmo_Implementation(float NewClipAmmo)
{
    ClipAmmo = NewClipAmmo;
}

bool AWeapon_Master::Multicast_SetClipAmmo_Validate(float NewClipAmmo)
{
    return true;
}
void AWeapon_Master::Multicast_SetMuzzleRotation_Implementation()
{
    auto Soldier = Cast<ASoldier>(Player);
    if (Soldier == nullptr)
        return;
    if (Soldier->IsLocallyControlled() == false)
        return;
    Soldier->SetMuzzleRotation();
}

bool AWeapon_Master::Multicast_SetMuzzleRotation_Validate()
{
    return true;
}


//void AWeapon_Master::Multicast_SendShot_Implementation()
//{
//    if (GetOwner()->HasAuthority())
//        return;
//
//    UE_LOG(LogTemp, Warning, TEXT("Multicast shot!!"));
//    FVector BulletScale;
//    // BulletScale.Set(0.1, 0.1, 0.1);
//    FTransform BulletTransform;
//
//    BulletTransform.SetLocation(SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation());
//    BulletTransform.SetRotation(MuzzleRotation_.Quaternion());
//    //BulletTransform.SetScale3D(BulletScale);
//
//
//    GetWorld()->SpawnActor<ABulletMaster>(BulletMasterClass, BulletTransform);
//}
//bool AWeapon_Master::Multicast_SendShot_Validate()
//{
//    return true;
//}
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
                if (UseRemainingAmmo == true)
                {
                    Multicast_SetClipAmmo(ClipAmmo + BagAmmo);
                    BagAmmo = 0;
                    AmmoNeeded = 0;
                    Reloading = false;
                }
                    else
                {
                    Multicast_SetClipAmmo(ClipAmmo + AmmoNeeded);
                    BagAmmo -= AmmoNeeded;
                    AmmoNeeded = 0;
                    Reloading = false;
                }

            }), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능

                        // 여기에 코드를 치면 된다.


    }
}

