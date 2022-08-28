// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile.h"



#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
AAbility_Projectile::AAbility_Projectile()
	:Super()
{
	ProjectileMovement_ = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement_"));

	ProjectileMovement_->InitialSpeed = 800;
	ProjectileMovement_->MaxSpeed = 800;
	ProjectileMovement_->ProjectileGravityScale = 0;
	ProjectileMovement_->bAutoActivate = false;
}
void AAbility_Projectile::BeginPlay()
{
	Super::BeginPlay();

	
}

void AAbility_Projectile::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();


}

void AAbility_Projectile::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();
	

}