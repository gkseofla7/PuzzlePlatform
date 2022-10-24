// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Master.h"
#include "NPC_Boss.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPC_Boss : public ANPC_Master
{
	GENERATED_BODY()
public:
	ANPC_Boss();
	void BeginPlay();
	void Tick(float DeltaTime);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Attack();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Attack(float RandomValue);
	UFUNCTION()
		virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);
	virtual void ChangeDamageColor() override;
	void ChangeOriginalColor();
	void Shot();
	void MeteorTarget();
	void Meteor(FVector MeteorLocationLocation);
	void StartFireBlast();
	void EndFireBlast();
	void FireBlast();

	void MeteorRandom();
	void ActivateParticle(bool NewActivate);
	UPROPERTY()
	class UIggyScorchAnimInstance* MyAnim;

	float AttackRange = 400;
	UMaterialInterface* ImpactedGoblinMaterial = nullptr;
	UMaterialInterface* GoblinMaterial = nullptr;
	UPROPERTY()
	TSubclassOf<class AScorchBomb> ScorchBombClass;
	UPROPERTY()
	TSubclassOf<class AMeteor> MeteorClass;
	UPROPERTY()
	TSubclassOf<class AMeteor_Target> MeteorTargetClass;
	FTimerHandle FireBlastTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* ParticleComponent;

	UPROPERTY()
	TSubclassOf<class AAbility>  Ability_Buff_Fire_Class;
};
