// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_DragonBlast.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
void AAbility_Projectile_DragonBlast::BeginPlay()
{
	Super::BeginPlay();

	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	//AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);
	AsPlayerAnimInstance->PlaySwordBlastMontage();


}

void AAbility_Projectile_DragonBlast::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();


}

void AAbility_Projectile_DragonBlast::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();

	UE_LOG(LogTemp, Warning, TEXT("DragonBlastActivateEffect"));

	if (PlayerRef->IsLocallyControlled() == true)
	{
		Server_SetVelocity(PlayerRef->GetMuzzleRotation().Vector() * 1500);
		//ProjectileMovement_->Velocity = (PlayerRef->GetMuzzleRotation()).Vector() * 1500;
	}
	ProjectileMovement_->Activate();
	//여기서 만약 컨트롤러가 있으면 
	if (PlayerRef->IsLocallyControlled() == true)
	{
		//Server_SetVelocity(PlayerRef->GetMuzzleRotation().Vector() * 1500);
		//ProjectileMovement_->Velocity = (PlayerRef->GetMuzzleRotation()).Vector() * 1500;
	}

	//ProjectileMovement_->SetVelocityInLocalSpace();
	ParticleSystemComponent->Activate(true);	
	AsPlayerAnimInstance->IsAttacking = false;

}