// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Master.h"
#include "NPC_Mob.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Mob : public ANPC_Master
{
	GENERATED_BODY()
public:
	ANPC_Mob();
	virtual void Attack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PlayImpactMontage();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_DamageImpact(float Damage);
};
