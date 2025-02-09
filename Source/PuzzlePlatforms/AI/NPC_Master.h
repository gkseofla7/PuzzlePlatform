// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "GameFramework/Character.h"
#include "NPC_Master.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)

UCLASS()
class PUZZLEPLATFORMS_API ANPC_Master : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Master();
	void PostInitializeComponents();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack();
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser);
	//void AttackCheck();

	//UFUNCTION()
	//	virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	//	class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetTarget(class ACharacter_Master* NewTarget);

	virtual void Die();

	void DestroyMonster();
	virtual void ChangeDamageColor();

	void Destroyed() override;
	void SpawnLoot(int Quantity);
	void DamageImpact(float Damage);
	void GetHelpedFromOthers(class ACharacter_Master* Target);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APatrolPath* PatrolPath;

	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	class UMonsterStatComponent* MonsterStat;//약간 애들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* HPWidget;//이것도 겹침
	bool bDead = false;
	UPROPERTY()
	AActor * AttackedPlayer;
	UPROPERTY()
	class UParticleSystem* ParticleTemplate;
	UPROPERTY()
	TSubclassOf<class ACharDamageText> CharDamageTextClass;
	UPROPERTY()
	TSubclassOf<class ASoulItem> SoulItemtClass;//이거 겹침
	UPROPERTY()
	class ACharacter_Master* Target;

};
