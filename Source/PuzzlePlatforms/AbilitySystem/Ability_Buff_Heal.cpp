// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Heal.h"
#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "ActorAbilities.h"

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

void AAbility_Buff_Heal::SetAbilityLevel()
{
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_Buff_Heal::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
			break;
		}
	}

	if (AbilityLevel == 1)
	{
		HealthUp = 2.;//0.5초당 2
	}
	else if (AbilityLevel == 2)
	{
		HealthUp = 3;
	}
	else if (AbilityLevel == 3)
	{
		HealthUp = 4;
	}
	else if (AbilityLevel == 4)
	{
		HealthUp = 5;
	}
	else if (AbilityLevel == 5)//5를 맥스로
	{
		HealthUp = 6;
	}
}