// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Debuff.h"
#include "HudUpDisplayWidget.h"
#include "BuffPanel_UI.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
void AAbility_Debuff::BeginPlay()
{
	PlayerRef = PlayerRef->TargetPlayer;
	Super::BeginPlay();
	bReplicates = true;
	auto warrior = Cast<AWarrior>(PlayerRef);
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	//AsPlayerAnimInstance->IsAttacking = true;
	//AsPlayerAnimInstance->OnActivateSpell.AddUObject(this, &AAbility_AoE::ActivateEffect);
	AsPlayerAnimInstance->PlayAoEAttackMontage();
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");

}

void AAbility_Debuff::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();

}




void AAbility_Debuff::TickBuff()
{

}



void AAbility_Debuff::ApplyBuff()
{
	Super::ApplyBuff();
}