// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Buff.h"
#include "Ability_Buff_SteamPack.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff_SteamPack : public AAbility_Buff
{
	GENERATED_BODY()
public:
	AAbility_Buff_SteamPack();
	virtual void BeginPlay() override;
	virtual void TickBuff();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_UnSteamPack();
};
