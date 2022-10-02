// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Dash.h"
#include "../../Warrior.h"
AAbility_Dash::AAbility_Dash()
	:Super()
{
	bReplicates = true;
}

void AAbility_Dash::BeginPlay()
{
	Super::BeginPlay();

	auto Warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;

	Warrior->Dash();

	PlayerRef->SetUsingSkill(false);
	Destroy();
}