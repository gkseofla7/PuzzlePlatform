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


void AWeapon_Master::Shot()//클라이언트에서 나감~
{
    auto Soldier = Cast<ASoldier>(Player);
    FRotator MuzzleRotator = Soldier->GetMuzzleRotation();
    if (!Soldier->HasAuthority())    Shot_Implementation(MuzzleRotator);

    ServerShot(MuzzleRotator);
}

void AWeapon_Master::ServerShot_Implementation(FRotator MuzzleRotator)
{
    MulticastShot(MuzzleRotator);
}

bool AWeapon_Master::ServerShot_Validate(FRotator MuzzleRotator)
{
    return true;
}

void AWeapon_Master::MulticastShot_Implementation(FRotator MuzzleRotator)
{
    if (Player && Player->IsLocallyControlled() && !Player->HasAuthority()) return;
    Shot_Implementation(MuzzleRotator);
}



void AWeapon_Master::Shot_Implementation(FRotator MuzzleRotator)
{
    AmmoCheck();//이건 서버에서 해줘야됨
    if (CanFire == true && ClipEmpty == false && Reloading == false)
    {
        SpendRound();//총알하나 쏘고~
        auto Soldier = Cast<ASoldier>(Player);
        FRotator MuzzleRotator = Soldier->GetMuzzleRotation();
        if (FireSound != NULL)//소리 다른애들한테도 해줘야됨
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        }
        if (MuzzlesParticle)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlesParticle, SkeletalMeshComponent->GetSocketTransform(FName("Muzzle")));
        }
        FTransform BulletTransform;
        BulletTransform.SetLocation(SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation());
        BulletTransform.SetRotation(MuzzleRotator.Quaternion());
        FActorSpawnParameters Params;
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Owner = GetOwner();
        SpawnInfo.Instigator = Cast<APawn>(Soldier);
        ABulletMaster* SpawnedProjectile = nullptr;
        if (bUseServerSideRewind)
        {

            if (Soldier->HasAuthority())
            {
                if (Soldier->IsLocallyControlled())//서버에 Player면 replicate projectile 사용할 필요 X
                {
                    //일반 replicated 총알만듬
                    SpawnedProjectile = GetWorld()->SpawnActor<ABulletMaster>(BulletMasterClass, BulletTransform, SpawnInfo);
                    SpawnedProjectile->bUseServerSideRewind = false;
                    SpawnedProjectile->Damage = Damage;
                    SpawnedProjectile->Shooter = Soldier;
                    //여기서 먼저 어느 방향 어느 위치인지 보내줘야됨
                }
                else// server, not locally controlled - spawn non-replicated projectile, no SSR
                {
                    SpawnedProjectile = GetWorld()->SpawnActor<ABulletMaster>(ServerSideRewindBulletMasterClass, BulletTransform, SpawnInfo);
                    SpawnedProjectile->bUseServerSideRewind = true;
                    SpawnedProjectile->Shooter = Soldier;
                }
            }
            else//client에서는?
            {
                if (Soldier->IsLocallyControlled())
                {
                    SpawnedProjectile = GetWorld()->SpawnActor<ABulletMaster>(ServerSideRewindBulletMasterClass, BulletTransform, SpawnInfo);
                    SpawnedProjectile->bUseServerSideRewind = true;
                    SpawnedProjectile->TraceStart = SkeletalMeshComponent->GetSocketTransform("Muzzle").GetLocation();
                    SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
                    SpawnedProjectile->Damage = Damage;
                    SpawnedProjectile->Shooter = Soldier;
                }
                else// Client, not locally controlled -spawn non-replicated projectile, no SSR
                {
                    SpawnedProjectile = GetWorld()->SpawnActor<ABulletMaster>(ServerSideRewindBulletMasterClass, BulletTransform, SpawnInfo);
                    SpawnedProjectile->bUseServerSideRewind = false;
                    SpawnedProjectile->Shooter = Soldier;
                }
            }
        }
        else//Weapon not using SSR
        {
            if (Soldier->HasAuthority())
            {

                SpawnedProjectile = GetWorld()->SpawnActor<ABulletMaster>(BulletMasterClass, BulletTransform, SpawnInfo);
                SpawnedProjectile->bUseServerSideRewind = false;
                SpawnedProjectile->Damage = Damage;
                SpawnedProjectile->Shooter = Soldier;
            }
        }
    }
}
void AWeapon_Master::SpendRound()
{
    UE_LOG(LogTemp, Warning, TEXT("Before Bullet %d"), ClipAmmo);
    ClipAmmo = FMath::Clamp(ClipAmmo - AmmoCost, 0, MaxClipAmmo);
    UE_LOG(LogTemp, Warning, TEXT("After Bullet %d"), ClipAmmo);
    if (HasAuthority())
    {
        Client_UpdateAmmo(ClipAmmo);
    }
    else
    {
        ++Sequence;
    }
}



void AWeapon_Master::Client_UpdateAmmo_Implementation(float ServerAmmo)
{
    if (HasAuthority())
    {
        return;
    }
    
    ClipAmmo = ServerAmmo;
    --Sequence;
    ClipAmmo -= Sequence;
}

bool AWeapon_Master::Client_UpdateAmmo_Validate(float ServerAmmo)
{
    return true;
}


void AWeapon_Master::Multicast_SetAmmo_Implementation(float NewClipAmmo, float NewBagAmmo)
{
    ClipAmmo = NewClipAmmo;
    BagAmmo = NewBagAmmo;
}

bool AWeapon_Master::Multicast_SetAmmo_Validate(float NewClipAmmo, float NewBagAmmo)
{
    return true;
}


void AWeapon_Master::Reload()//서버에서만 동작
{
    AmmoCheck();
    if (CanReload)
    {
        //Reloading = true;
        //UKismetSystemLibrary::Delay(GetWorld(), ReloadDelay);

        FTimerHandle WaitHandle;
        float WaitTime = ReloadDelay; //시간을 설정하고
        GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
            {      
                if (UseRemainingAmmo == true)
                {
                    Multicast_SetAmmo(ClipAmmo + BagAmmo, 0);
                    AmmoNeeded = 0;
                    //Reloading = false;
                }
                else
                {
                    Multicast_SetAmmo(ClipAmmo + AmmoNeeded, BagAmmo-AmmoNeeded);
                    AmmoNeeded = 0;
                    //Reloading = false;
                }

            }), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능

                        // 여기에 코드를 치면 된다.


    }
}

