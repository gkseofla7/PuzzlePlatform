// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndTestDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UGoblinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGoblinAnimInstance();
	void PlaySwordAttackMontage();
	UFUNCTION()
		void AnimNotify_AttackHitCheck();
	UFUNCTION()
		void AnimNotify_EndTest();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* SwordAttackMontage;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnEndTestDelegate OnEndTestDelegate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;
};
