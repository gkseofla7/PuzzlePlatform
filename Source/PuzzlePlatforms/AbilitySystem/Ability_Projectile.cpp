// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile.h"



#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"
AAbility_Projectile::AAbility_Projectile()
	:Super()
{
	bReplicates = true;
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

void AAbility_Projectile::Server_SetVelocity_Implementation(FVector NewVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("InServer"))
		NetMulticast_SetVelocity(NewVelocity);
}

void AAbility_Projectile::NetMulticast_SetVelocity_Implementation(FVector NewVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("InClient"))
		ProjectileMovement_->Velocity = NewVelocity;
}


bool AAbility_Projectile::Server_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}

bool AAbility_Projectile::NetMulticast_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}