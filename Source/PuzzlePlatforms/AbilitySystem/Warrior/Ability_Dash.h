// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "Ability_Dash.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Dash : public AAbility
{
	GENERATED_BODY()
	
public:
	AAbility_Dash();
protected:
	void BeginPlay();
};
