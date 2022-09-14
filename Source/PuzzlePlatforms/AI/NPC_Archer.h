// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Master.h"
#include "NPC_Archer.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Archer : public ANPC_Master
{
	GENERATED_BODY()

public:
	ANPC_Archer();
	void BeginPlay();
	virtual void Attack();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack();
	void ArrowShot();

public:
	UPROPERTY()
		class UArcherAnimInstance* MyAnim;
	float AttackRange = 500;
};
