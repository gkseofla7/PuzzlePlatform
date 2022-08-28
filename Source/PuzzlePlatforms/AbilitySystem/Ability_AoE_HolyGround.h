// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_AoE.h"
#include "Ability_AoE_HolyGround.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_AoE_HolyGround : public AAbility_AoE
{
	GENERATED_BODY()
public:
	AAbility_AoE_HolyGround();
	//virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
	void TickEffect();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDecalComponent* DecalComponent;

	float LifeSpan = 5;
	float DoTTimer = 5;
};
