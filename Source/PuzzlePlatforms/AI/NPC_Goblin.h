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
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack();


public:
	UPROPERTY()
		class UGoblinAnimInstance* MyAnim;
	float AttackRange = 200;
};
