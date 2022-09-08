// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_SteamPack.h"
#include "../../PuzzlePlatformsCharacter.h"
#include"../../Soldier.h"
AAbility_Buff_SteamPack::AAbility_Buff_SteamPack()
	:Super()
{
	BuffTickRate = 9;
}

void AAbility_Buff_SteamPack::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerRef->IsLocallyControlled())
	{
		auto Soldier = Cast<ASoldier>(PlayerRef);
		Soldier->SteamPack();
	}
	
}
void AAbility_Buff_SteamPack::TickBuff()
{
	if (PlayerRef->IsLocallyControlled())
	{
		auto Soldier = Cast<ASoldier>(PlayerRef);
		Soldier->UnSteamPack();
	}
	Destroy();
}
