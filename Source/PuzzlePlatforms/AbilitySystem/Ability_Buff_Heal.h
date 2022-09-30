// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Buff_Master.h"
#include "Ability_Buff_Heal.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff_Heal : public AAbility_Buff_Master
{
	GENERATED_BODY()
public:
		AAbility_Buff_Heal();
		virtual void BeginPlay() override;
		virtual void TickBuff() override;

		float HealthUp = 2.;
};
