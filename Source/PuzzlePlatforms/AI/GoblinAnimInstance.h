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
	void PlayDeathMontage();
	void PlayImpactMontage();
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* SwordAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* ImpactMontage;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;
};
