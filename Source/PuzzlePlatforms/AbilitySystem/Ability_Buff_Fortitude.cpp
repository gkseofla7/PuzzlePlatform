// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Fortitude.h"
#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
AAbility_Buff_Fortitude::AAbility_Buff_Fortitude()
	:Super()
{

}
void AAbility_Buff_Fortitude::BeginPlay()
{
	Super::BeginPlay();
	auto warrior = Cast<AWarrior>(PlayerRef);

	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	//AsPlayerAnimInstance->OnActivateSpell.AddUObject(this, &AAbility_AoE::ActivateEffect);

	AsPlayerAnimInstance->PlayAoEAttackMontage();
}