// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Fireball.h"

#include "../../Warrior.h"
#include "../../AnimInstance/PlayerAnimInstance.h"
#include "../ActorAbilities.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AAbility_Projectile_Fireball::AAbility_Projectile_Fireball()
	:Super()
{
	AbilityRoot->OnComponentBeginOverlap.AddDynamic(this, &AAbility_Projectile_Fireball::OnOverlapBegin);

	AbilityRoot->SetNotifyRigidBodyCollision(true);
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTemplate = ParticleAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BigFireParticleTemplateAsset(TEXT("/Game/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong"));
	if (ParticleAsset.Succeeded())
	{
		BigFireParticleTemplate = BigFireParticleTemplateAsset.Object;
	}
}
void AAbility_Projectile_Fireball::BeginPlay()
{
	Super::BeginPlay();

	auto warrior = Cast<AWarrior>(PlayerRef);

	bReplicates = true;
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimRef);
	AsPlayerAnimInstance->Montage_JumpToSection(FName("Defualt"), AsPlayerAnimInstance->FireballAttackMontage);
	if(PlayerRef->IsLocallyControlled())
	{ 
		AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	}

	//AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);
	AsPlayerAnimInstance->PlayFireballAttackMontage();
	//AsPlayerAnimInstance->OnFireBall.Add()

}

void AAbility_Projectile_Fireball::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();

	Server_PlayNextAnimation();
	//AsPlayerAnimInstance->PlayFireballAttackMontage();

}

void AAbility_Projectile_Fireball::ActivateEffect_Implementation()
{

	if (PlayerRef->IsLocallyControlled() == false)
		return;

	//즉 클라이언트에서만 진행
	Super::ActivateEffect_Implementation();

	Server_DetachAbilityFromPlayer();//모두 일단 띄어냄
	Server_SetLocation(GetActorLocation());
	Server_SetVelocity(PlayerRef->GetMuzzleRotation().Vector() * 1500);
	Server_SetFireballParticle();
	Server_Activate();
	//초기값 위치 다 맞춤



}

void AAbility_Projectile_Fireball::Server_SetFireballParticle_Implementation()
{
	NetMulticast_SetFireballParticle();
}

bool AAbility_Projectile_Fireball::Server_SetFireballParticle_Validate()
{
	return true;
}

void AAbility_Projectile_Fireball::NetMulticast_SetFireballParticle_Implementation()
{
	ParticleSystemComponent->SetTemplate(BigFireParticleTemplate);
}

bool AAbility_Projectile_Fireball::NetMulticast_SetFireballParticle_Validate()
{
	return true;
}






void AAbility_Projectile_Fireball::NetMulticast_Spark_Implementation(FVector Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate,Location, FRotator(0, 0, 0));
}

bool AAbility_Projectile_Fireball::NetMulticast_Spark_Validate(FVector Location)
{
	return true;
}

void AAbility_Projectile_Fireball::Server_PlayNextAnimation_Implementation()
{
	NetMulticast_PlayNextAnimation();
}

bool AAbility_Projectile_Fireball::Server_PlayNextAnimation_Validate()
{
	return true;
}

void AAbility_Projectile_Fireball::NetMulticast_PlayNextAnimation_Implementation()
{

	ABCHECK(AsPlayerAnimInstance != nullptr);
	if (HasAuthority() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER Shot"));
	}
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->FireballAttackMontage);
}

bool AAbility_Projectile_Fireball::NetMulticast_PlayNextAnimation_Validate()
{
	return true;
}


void AAbility_Projectile_Fireball::NetMulticast_DeactivateParticle_Implementation()
{
	ParticleSystemComponent->Deactivate();
}

bool AAbility_Projectile_Fireball::NetMulticast_DeactivateParticle_Validate()
{
	return true;
}

//void AAbility_Projectile_Fireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	NetMulticast_Spark(Hit.Location);
//	Destroy();
//}

void AAbility_Projectile_Fireball::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
		return;
	UE_LOG(LogTemp, Warning, TEXT("Hit %s, %s %s") ,*OverlappedComp->GetName(),*OtherActor->GetName(), *OtherComp->GetName());

	if (HasAuthority() == true)
	{
		NetMulticast_Spark(GetActorLocation());
		auto Player = Cast<ACharacter>(OtherActor);
		auto PuzzleCharacter = Cast<ACharacter_Master>(OtherActor);
		bool Check = false;
		if (PuzzleCharacter != nullptr)
		{
			if (PuzzleCharacter->TeamNum == PlayerRef->TeamNum)
				Check = true;

		}
		if (Player != nullptr&&Check==false)//같은팀이아닌 Character면 데미지
		{

			UGameplayStatics::ApplyDamage(Player, DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

		}
	}

	if (AnimationEnd == true)
	{
		Destroy();
	}
	else
	{
		NetMulticast_DeactivateParticle();
		NeedToDestroy = true;
	}
}

void AAbility_Projectile_Fireball::SetAbilityLevel()
{
	//쓸때마다 불러옴
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_Projectile_Fireball::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
		}
	}
	if (AbilityLevel == 1)
	{
		DamageAmount = 25.;
	}
	else if (AbilityLevel == 2)
	{
		DamageAmount = 50;
	}
	else if (AbilityLevel == 3)
	{
		DamageAmount = 75;
	}
	else if (AbilityLevel == 4)
	{
		DamageAmount = 120;
	}
	else if (AbilityLevel == 5)//5를 맥스로
	{
		DamageAmount = 150;
	}

}