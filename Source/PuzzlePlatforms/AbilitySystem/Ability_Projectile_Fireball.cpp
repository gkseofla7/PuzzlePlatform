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
	bReplicates = true;
}
void AAbility_Projectile_Fireball::BeginPlay()
{
	Super::BeginPlay();
	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Second One"));
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


void AAbility_Projectile_Fireball::Server_SetVelocity_Implementation(FVector NewVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("InServer"))
	NetMulticast_SetVelocity(NewVelocity);
}

void AAbility_Projectile_Fireball::NetMulticast_SetVelocity_Implementation(FVector NewVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("InClient"))
	ProjectileMovement_->Velocity = NewVelocity;
}


bool AAbility_Projectile_Fireball::Server_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}

bool AAbility_Projectile_Fireball::NetMulticast_SetVelocity_Validate(FVector NewVelocity)
{
	return true;
}