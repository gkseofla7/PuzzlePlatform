// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Ability_Buff_Master.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnEndBuffDelegate);

UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff_Master : public AAbility
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();

	virtual void CastAbility_Implementation() override;

public:
	virtual void ApplyBuff();
	virtual void TickBuff();
	void ClearBuff();

	void ClearDuplicates();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;
	float BuffLifeSpan = 10;
	float BuffTickRate = .5;
	FOnEndBuffDelegate OnEndBuffDelegate;
};
