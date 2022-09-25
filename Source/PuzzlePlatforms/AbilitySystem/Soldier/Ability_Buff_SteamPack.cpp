// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_SteamPack.h"
#include "../../Character_Master.h"
#include"../../Soldier.h"
AAbility_Buff_SteamPack::AAbility_Buff_SteamPack()
	:Super()
{
	BuffTickRate = 9;
}

void AAbility_Buff_SteamPack::BeginPlay()
{
	Super::BeginPlay();
	BuffLifeSpan = 10;
	bIsRepeat = false;
	if (PlayerRef->IsLocallyControlled())
	{
		auto Soldier = Cast<ASoldier>(PlayerRef);
		Soldier->SteamPack();
	}
	
}
void AAbility_Buff_SteamPack::TickBuff()//이건 시간지나고 나한테 실행해야됨
{
	NetMulticast_UnSteamPack();

	Destroy();
}

void AAbility_Buff_SteamPack::NetMulticast_UnSteamPack_Implementation()
{
	auto Soldier = Cast<ASoldier>(PlayerRef);
	if (Soldier != nullptr)
	{
		Soldier->UnSteamPack();
	}
}

bool AAbility_Buff_SteamPack::NetMulticast_UnSteamPack_Validate()
{
	return true;
}
