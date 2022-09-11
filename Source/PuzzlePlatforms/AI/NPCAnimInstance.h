// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UNPCAnimInstance();
	void PlaySwordAttackMontage();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* SwordAttackMontage;
};
