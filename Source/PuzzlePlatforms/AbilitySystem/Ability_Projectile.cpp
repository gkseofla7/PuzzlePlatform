// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile.h"



#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
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
	NetMulticast_SetVelocity(NewVelocity);
}

void AAbility_Projectile::NetMulticast_SetVelocity_Implementation(FVector NewVelocity)
{
	ProjectileMovement_->Velocity = NewVelocity;
}


void AAbility_Projectile::Server_AddLocation_Implementation(FVector RelativeLocation)
{
	NetMulticast_AddLocation(RelativeLocation);
}

void AAbility_Projectile::NetMulticast_AddLocation_Implementation(FVector RelativeLocation)
{
	AbilityRoot->AddLocalOffset(RelativeLocation);
}

bool AAbility_Projectile::Server_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}

bool AAbility_Projectile::NetMulticast_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}

bool AAbility_Projectile::Server_AddLocation_Validate(FVector RelativeLocation)
{
	return true;
}

bool AAbility_Projectile::NetMulticast_AddLocation_Validate(FVector RelativeLocation)
{
	return true;
}