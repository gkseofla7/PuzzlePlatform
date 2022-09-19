// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Fireball.h"

#include "../../Warrior.h"
#include "../../AnimInstance/PlayerAnimInstance.h"

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
	Server_Activate();
	//초기값 위치 다 맞춤



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
	//UE_LOG(LogTemp, Warning, TEXT("Hit %s, %s %s") ,*OverlappedComp->GetName(),*OtherActor->GetName(), *OtherComp->GetName());

	if (HasAuthority() == true)
	{
		NetMulticast_Spark(OtherActor->GetActorLocation());
		auto Player = Cast<ACharacter>(OtherActor);
		auto PuzzleCharacter = Cast<APuzzlePlatformsCharacter>(OtherActor);
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

