// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
AAbility_Projectile::AAbility_Projectile()
{
	ProjectileMovement_ = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement_"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ProjectileMovement_->InitialSpeed = 800;
	ProjectileMovement_->MaxSpeed = 800;
	ProjectileMovement_->ProjectileGravityScale = 0;
	ProjectileMovement_->bAutoActivate = false;
}
void AAbility_Projectile::BeginPlay()
{
	Super::BeginPlay();
	auto warrior = Cast<AWarrior>(PlayerRef);
	
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	AsPlayerAnimInstance->Montage_JumpToSection(FName("Defualt"), AsPlayerAnimInstance->FireballAttackMontage);
	AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	AsPlayerAnimInstance->OnFireBall.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);
	AsPlayerAnimInstance->PlayFireballAttackMontage();
	//AsPlayerAnimInstance->OnFireBall.Add()
	
}

void AAbility_Projectile::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("Second One"));
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->FireballAttackMontage);
	//AsPlayerAnimInstance->PlayFireballAttackMontage();

}

void AAbility_Projectile::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();
	

	ProjectileMovement_->Activate();
	AsPlayerAnimInstance->IsAttacking = false;

}