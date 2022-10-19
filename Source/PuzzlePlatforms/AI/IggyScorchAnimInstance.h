// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Animation/AnimInstance.h"
#include "IggyScorchAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnShotDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UIggyScorchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UIggyScorchAnimInstance();
	void UpdateAnimation(float DeltaSeconds, bool bNeedsValidRootMotion, EUpdateAnimationFlag UpdateFlag);
	void PlayAttackMontage();
	UFUNCTION()
	void AnimNotify_Shot();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	FOnShotDelegate OnShotDelegate;
	
};
