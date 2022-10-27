// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_DragonBlast.h"
#include "../ActorAbilities.h"
#include "../../Warrior.h"
#include "../../AnimInstance/PlayerAnimInstance.h"


#include "GameFramework/ProjectileMovementComponent.h"

AAbility_Projectile_DragonBlast::AAbility_Projectile_DragonBlast()
	:Super()
{
	//AbilityRoot->OnComponentHit.AddDynamic(this, &AAbility_Projectile_Fireball::OnHit);
	AbilityRoot->OnComponentBeginOverlap.AddDynamic(this, &AAbility_Projectile_DragonBlast::OnOverlapBegin);

	AbilityRoot->SetNotifyRigidBodyCollision(true);
	bReplicates = true;

	static ConstructorHelpers::FClassFinder<AAbility> Ability_Buff_Heal_BPClass(TEXT("/Game/AbilitySystem/Buff/BP_Ability_Buff_Fire"));
	if (Ability_Buff_Heal_BPClass.Succeeded())
	{
		Ability_Buff_Heal_Class = Ability_Buff_Heal_BPClass.Class;

		//EquippedItem->Get
	}
	//Ability_Buff_Heal_Class
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
	if (PlayerRef->IsLocallyControlled())
	{
		AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	}
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
	Server_PlayNextAnimation();

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

			auto Player = Cast<ACharacter>(OtherActor);
			auto PuzzleCharacter = Cast<ACharacter_Master>(OtherActor);
			if (PuzzleCharacter != nullptr && PuzzleCharacter->TeamNum == PlayerRef->TeamNum)
				return;
			if ((Player != nullptr && Player != PlayerRef) )
			{
				UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, PlayerRef->GetController(), this, UDamageType::StaticClass());
				//고블린도 진행해야됨
				if(PuzzleCharacter!=nullptr)
					PuzzleCharacter->ReplicateComponent->AbilitySpawn(Ability_Buff_Heal_Class);
				SweepArray.Add(OtherActor);
			}
			
		}
	}
}



void AAbility_Projectile_DragonBlast::Server_PlayNextAnimation_Implementation()
{
	NetMulticast_PlayNextAnimation();
}

bool AAbility_Projectile_DragonBlast::Server_PlayNextAnimation_Validate()
{
	return true;
}


void AAbility_Projectile_DragonBlast::NetMulticast_PlayNextAnimation_Implementation()
{

	ABCHECK(AsPlayerAnimInstance != nullptr);
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->SwordBlastMontage);
}

bool AAbility_Projectile_DragonBlast::NetMulticast_PlayNextAnimation_Validate()
{
	return true;
}

void AAbility_Projectile_DragonBlast::SetAbilityLevel()
{
	//쓸때마다 불러옴
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_Projectile_DragonBlast::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
		}
	}

	if (AbilityLevel == 1)
	{
		DamageAmount = 40.;
	}
	else if (AbilityLevel == 2)
	{
		DamageAmount = 70;
	}
	else if (AbilityLevel == 3)
	{
		DamageAmount = 150;
	}
	else if (AbilityLevel == 4)
	{
		DamageAmount = 200;
	}
	else if (AbilityLevel == 5)//5를 맥스로
	{
		DamageAmount = 300;
	}
}