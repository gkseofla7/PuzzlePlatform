// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "HudUpDisplayWidget.h"
#include "BuffPanel_UI.h"
void AAbility_Buff::BeginPlay()
{
	Super::BeginPlay();
	ClearDuplicates();
	bReplicates = true;
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");

}

void AAbility_Buff::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
}

void AAbility_Buff::ApplyBuff()
{
	Super::ApplyBuff();

}


