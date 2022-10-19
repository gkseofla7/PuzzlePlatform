// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Mob.h"
#include "NPC_Archer.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Archer : public ANPC_Mob
{
	GENERATED_BODY()

public:
	ANPC_Archer();
	void BeginPlay();
	virtual void Attack();
	virtual void ChangeDamageColor() override;
	virtual void PlayImpactMontage() override;
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack();

	void ArrowShot();
	UFUNCTION()
		virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);
	virtual void Die() override;
	void ChangeOriginalColor();
public:
	UPROPERTY()
	class UArcherAnimInstance* MyAnim;
	float AttackRange = 500;

	UPROPERTY()
	TSubclassOf<class AArrowMaster> ArrowMasterClass;
	UPROPERTY()
	UMaterialInterface* ImpactedArcherMaterial = nullptr;
	UPROPERTY()
	UMaterialInterface* ArcherMaterial = nullptr;
	UPROPERTY()
	UMaterialInterface* ImpactedArcherBodyMaterial = nullptr;
	UPROPERTY()
	UMaterialInterface* ArcherBodyMaterial = nullptr;
};
