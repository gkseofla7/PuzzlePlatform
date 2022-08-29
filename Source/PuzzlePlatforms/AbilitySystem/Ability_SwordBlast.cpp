// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SwordBlast.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

void AAbility_SwordBlast::BeginPlay()
{
	Super::BeginPlay();

	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	//AsPlayerAnimInstance->IsAttacking = true;

	AsPlayerAnimInstance->PlaySwordBlastMontage();

}