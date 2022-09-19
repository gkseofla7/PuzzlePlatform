// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Buff_Master.h"
#include "Ability_Buff_Fortitude.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff_Fortitude : public AAbility_Buff_Master
{
	GENERATED_BODY()
public:
		AAbility_Buff_Fortitude();
		virtual void BeginPlay() override;
};
