// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_DragonBlast.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

#include "GameFramework/ProjectileMovementComponent.h"
void AAbility_Projectile_DragonBlast::BeginPlay()
{
	Super::BeginPlay();
	
	//bCanEverTick = true;
	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	AsPlayerAnimInstance->Montage_JumpToSection(FName("Defualt"), AsPlayerAnimInstance->SwordBlastMontage);
	//AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::ActivateEffect);
	AsPlayerAnimInstance->OnFireDragonBlastDelegate.AddUObject(this, &AAbility_Projectile::DetachAbilityFromPlayer);
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
	AsPlayerAnimInstance->Montage_JumpToSection(FName("EndCast"), AsPlayerAnimInstance->SwordBlastMontage);

}

void AAbility_Projectile_DragonBlast::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();

	UE_LOG(LogTemp, Warning, TEXT("DragonBlastActivateEffect"));

	if (PlayerRef->IsLocallyControlled() == true)
	{
		Server_AddLocation(PlayerRef->GetActorForwardVector() * 100);
		Server_SetVelocity(FVector(PlayerRef->GetMuzzleRotation().Vector().X, PlayerRef->GetMuzzleRotation().Vector().Y,0)* 1500);

		//ProjectileMovement_->Velocity = (PlayerRef->GetMuzzleRotation()).Vector() * 1500;
	}
	ProjectileMovement_->Activate(true);
	//여기서 만약 컨트롤러가 있으면 


	//ProjectileMovement_->SetVelocityInLocalSpace();
	ParticleSystemComponent->Activate(true);	
	AsPlayerAnimInstance->IsAttacking = false;

}