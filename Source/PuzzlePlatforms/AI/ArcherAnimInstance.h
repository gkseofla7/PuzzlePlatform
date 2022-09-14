// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Animation/AnimInstance.h"
#include "ArcherAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnArrowShotDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UArcherAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UArcherAnimInstance();
	void PlayArrowAttackMontage();
	UFUNCTION()
		void AnimNotify_ArrowShot();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* ArrowAttackMontage;
	FOnArrowShotDelegate OnArrowShotDelegate;
};
