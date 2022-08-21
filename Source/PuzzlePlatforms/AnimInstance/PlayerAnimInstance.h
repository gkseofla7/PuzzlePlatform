// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHangMovePlaceDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//이건 Tick마다 호출하는 함수

	void PlaySwordAttackMontage();
	void PlayHangToCrouchMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	UAnimMontage* GetHangToCrouchMontage() { return HangToCrouchMontage; }

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
	FOnHangMovePlaceDelegate OnHangMovePlace;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		bool IsAttacking = false;
private:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrenPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsClimbing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float MoveUpDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float MoveRightLeft;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HangToCrouchMontage;

	UFUNCTION()
		void AnimNotify_NextAttackCheck();
	UFUNCTION()
		void AnimNotify_EndAttack();
	UFUNCTION()
		void AnimNotify_MovePlace();
	FName GetAttackMontageSectionName(int32 Section);
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();


};
