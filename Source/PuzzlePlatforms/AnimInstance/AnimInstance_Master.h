// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Master.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UAnimInstance_Master : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Attack)
	bool IsAttacking = false;
};
