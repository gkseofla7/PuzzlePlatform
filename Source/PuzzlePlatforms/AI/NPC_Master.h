// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack();
	//void AttackCheck();

	UFUNCTION()
		virtual void EndAnimation(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APatrolPath* PatrolPath;

	FOnAttackEndDelegate OnAttackEnd;
	UPROPERTY()
	class UNPCAnimInstance* MyAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	class UMonsterStatComponent* MonsterStat;//약간 애들

	

};
