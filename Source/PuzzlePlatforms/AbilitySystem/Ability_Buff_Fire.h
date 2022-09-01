// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Buff.h"
#include "Ability_Buff_Fire.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff_Fire : public AAbility_Buff
{
	GENERATED_BODY()
public:
	AAbility_Buff_Fire();
	virtual void BeginPlay() override;
	virtual void TickBuff();
};
