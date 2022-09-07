// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Projectile.h"
#include "Ability_Projectile_Missile.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile_Missile : public AAbility_Projectile
{
	GENERATED_BODY()

public:
	AAbility_Projectile_Missile();
	virtual void BeginPlay();
	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DelayCollision();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* MissileComponent; 
	class ASoldier* SoldierRef;
};
