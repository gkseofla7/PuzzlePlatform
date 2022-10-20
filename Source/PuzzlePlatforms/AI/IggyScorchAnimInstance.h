// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Animation/AnimInstance.h"
#include "IggyScorchAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnShotDelegate);
//DECLARE_MULTICAST_DELEGATE(FOnFireMeteorDelegate);
UCLASS()
class PUZZLEPLATFORMS_API UIggyScorchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UIggyScorchAnimInstance();
	
	void PlayAttackMontage();
	void PlayFireBlastMontage();
	void PlayMateorMontage();
	void 	NativeUpdateAnimation(float DeltaSeconds);
	UFUNCTION()
	void AnimNotify_Shot();
	UFUNCTION()
		void AnimNotify_FireStart();
	UFUNCTION()
		void AnimNotify_FireEnd();
	UFUNCTION()
		void AnimNotify_FireMeteor();
	UFUNCTION()
		void AnimNotify_DownCheck();
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* FireBlastMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
		UAnimMontage* MeteorMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FullBody = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator AimRotation;
	FOnShotDelegate OnShotDelegate;
	FOnShotDelegate OnFireMeteorDelegate;
	float TotalTimeForMeteor = 0.7f;
	float CurrentTimeForMeteor = 0.f;
	FRotator StartRotator;
	FRotator EndRotator;

	bool bMateor = false;
	bool bMateorStart = false;

	
};
