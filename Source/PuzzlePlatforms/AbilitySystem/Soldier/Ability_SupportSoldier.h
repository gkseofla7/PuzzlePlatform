// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "Ability_SupportSoldier.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_SupportSoldier : public AAbility
{
	GENERATED_BODY()
public:
	AAbility_SupportSoldier();

	virtual void BeginPlay() override;


	void SpawnSoldierBot();


	TSubclassOf<class ABot>BotClass;
};
