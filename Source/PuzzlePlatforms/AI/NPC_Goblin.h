// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Mob.h"
#include "NPC_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Goblin : public ANPC_Mob
{
	GENERATED_BODY()
	

public:
	ANPC_Goblin();
	void BeginPlay();
	virtual void Attack();
	void AttackCheck();
	virtual void PlayImpactMontage();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack();

	UFUNCTION()
		virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);
	virtual void ChangeDamageColor() override;
	void ChangeOriginalColor();
	virtual void Die() override;
public:
	UPROPERTY()
		class UGoblinAnimInstance* MyAnim;
	float AttackRange = 200;
	UMaterialInterface* ImpactedGoblinMaterial = nullptr;
	UMaterialInterface* GoblinMaterial = nullptr;
};
