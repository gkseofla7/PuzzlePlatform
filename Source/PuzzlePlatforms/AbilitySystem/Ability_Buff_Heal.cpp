// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Heal.h"
#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"
AAbility_Buff_Heal::AAbility_Buff_Heal()
	:Super()
{

}
void AAbility_Buff_Heal::BeginPlay()
{
	Super::BeginPlay();
	auto warrior = Cast<AWarrior>(PlayerRef);

}

void AAbility_Buff_Heal::TickBuff()
{
	Super::TickBuff();
	auto MyStat = PlayerRef->CharacterStatRef;
	MyStat->Server_SetHP(HealthUp + MyStat->CurrentHP);
}