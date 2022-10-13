// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Master.h"
#include "NPC_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Goblin : public ANPC_Master
{
	GENERATED_BODY()
	

public:
	ANPC_Goblin();
	void BeginPlay();
	virtual void Attack();
	void AttackCheck();
	virtual void TakeDamage_Implementation() override;
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack();
	UFUNCTION()
		virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);

	virtual void Die() override;
public:
	UPROPERTY()
		class UGoblinAnimInstance* MyAnim;
	float AttackRange = 200;
};
