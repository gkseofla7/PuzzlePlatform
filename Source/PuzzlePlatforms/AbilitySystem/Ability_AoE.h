// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Ability_AoE.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_AoE : public AAbility
{
	GENERATED_BODY()
	

public:
	AAbility_AoE();
	virtual void BeginPlay();
	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
	



public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;

};
