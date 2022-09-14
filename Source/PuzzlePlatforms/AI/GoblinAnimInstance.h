// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UGoblinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGoblinAnimInstance();
	void PlaySwordAttackMontage();
	UFUNCTION()
		void AnimNotify_AttackHitCheck();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* SwordAttackMontage;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;
};
