// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_DragonBlast.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"


#include "GameFramework/ProjectileMovementComponent.h"

AAbility_Projectile_DragonBlast::AAbility_Projectile_DragonBlast()
	:Super()
{
	//AbilityRoot->OnComponentHit.AddDynamic(this, &AAbility_Projectile_Fireball::OnHit);
	AbilityRoot->OnComponentBeginOverlap.AddDynamic(this, &AAbility_Projectile_DragonBlast::OnOverlapBegin);

	AbilityRoot->SetNotifyRigidBodyCollision(true);
	bReplicates = true;

	static ConstructorHelpers::FClassFinder<AAbility> Ability_Buff_Fortitude_BPClass(TEXT("/Game/AbilitySystem/Buff/BP_Ability_Buff_Fire"));
	if (Ability_Buff_Fortitude_BPClass.Succeeded())
	{
		Ability_Buff_Fortitude_Class = Ability_Buff_Fortitude_BPClass.Class;

		//EquippedItem->Get
	}
	//Ability_Buff_Fortitude_Class
}

void AAbility_Projectile_DragonBlast::BeginPlay()
{
	Super::BeginPlay();
	
	//bCanEverTick = true;
	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	AsPlayerAnimInstance->Montage_JumpToSection(FName("Defualt"), AsPlayerAnimInstance->SwordBlastMontage);
	//AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	//AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);

	AsPlayerAnimInstance->PlaySwordBlastMontage();


}

void AAbility_Projectile_DragonBlast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (Toggle == false && ParticleSystemComponent->IsActive() == true)
		{
			Toggle = true;
		}
		if (Toggle == true && ParticleSystemComponent->IsActive() == false)
		{
			Destroy();
		}
	}


}

void AAbility_Projectile_DragonBlast::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->SwordBlastMontage);

}



void AAbility_Projectile_DragonBlast::ActivateEffect_Implementation()
{

	if (PlayerRef->IsLocallyControlled() == false)
		return;
	//즉 클라이언트에서만 진행
	Super::ActivateEffect_Implementation();

	Server_DetachAbilityFromPlayer();//모두 일단 띄어냄

	//Server_AddLocation(PlayerRef->GetActorForwardVector() * 100);
	Server_SetVelocity(FVector(PlayerRef->GetMuzzleRotation().Vector().X, PlayerRef->GetMuzzleRotation().Vector().Y, 0) * 1500);
	Server_SetLocation(GetActorLocation());
	AbilityRoot->AddLocalOffset(PlayerRef->GetActorForwardVector() * 100);
	Server_Activate();
	//초기값 위치 다 맞춤


}

void AAbility_Projectile_DragonBlast::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		int32 index = SweepArray.Find(OtherActor);
		if (index == -1)
		{
			auto Player = Cast<APuzzlePlatformsCharacter>(OtherActor);
			if (Player != nullptr && Player != PlayerRef)
			{
				UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, PlayerRef->GetController(), this, UDamageType::StaticClass());
				UE_LOG(LogTemp, Warning, TEXT("Got DragonBalst"))
				Player->DaerimMotionReplicator->AbilitySpawn(Ability_Buff_Fortitude_Class);
				SweepArray.Add(OtherActor);
			}
		}
	}
}


