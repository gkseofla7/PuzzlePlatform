// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//이건 Tick마다 호출하는 함수

	void PlaySwordAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);


public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
	bool IsAttacking = false;
private:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrenPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordAttackMontage;

	UFUNCTION()
		void AnimNotify_NextAttackCheck();
	UFUNCTION()
		void AnimNotify_EndAttack();
	FName GetAttackMontageSectionName(int32 Section);
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();


};
