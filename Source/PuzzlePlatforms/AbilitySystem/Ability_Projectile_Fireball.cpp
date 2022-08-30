// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Fireball.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AAbility_Projectile_Fireball::AAbility_Projectile_Fireball()
	:Super()
{
	AbilityRoot->OnComponentHit.AddDynamic(this, &AAbility_Projectile_Fireball::OnHit);
	AbilityRoot->SetNotifyRigidBodyCollision(true);
	//SetNotifyRigidBodyCollision(true);
	//OnActorHit.AddDynamic(this, &AAbility_Projectile_Fireball::OnHit);
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
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	AsPlayerAnimInstance->Montage_JumpToSection(FName("Defualt"), AsPlayerAnimInstance->FireballAttackMontage);
	AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);
	AsPlayerAnimInstance->PlayFireballAttackMontage();
	//AsPlayerAnimInstance->OnFireBall.Add()

}

void AAbility_Projectile_Fireball::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->FireballAttackMontage);
	//AsPlayerAnimInstance->PlayFireballAttackMontage();

}

void AAbility_Projectile_Fireball::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();


	ProjectileMovement_->Activate();
	//여기서 만약 컨트롤러가 있으면 
	if(PlayerRef->IsLocallyControlled()==true)
	{
		Server_SetVelocity(PlayerRef->GetMuzzleRotation().Vector() * 1500);
		//ProjectileMovement_->Velocity = (PlayerRef->GetMuzzleRotation()).Vector() * 1500;
	}

	//ProjectileMovement_->SetVelocityInLocalSpace();
	AsPlayerAnimInstance->IsAttacking = false;

}






void AAbility_Projectile_Fireball::NetMulticast_Spark_Implementation(FVector Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate,Location, FRotator(0, 0, 0));
}

bool AAbility_Projectile_Fireball::NetMulticast_Spark_Validate(FVector Location)
{
	return true;
}

void AAbility_Projectile_Fireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	NetMulticast_Spark(Hit.Location);
	Destroy();
}

//void AAbility_Projectile_Fireball::ReceiveHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
//{
//
//}