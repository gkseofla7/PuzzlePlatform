// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Buff_Master.h"
#include "Ability_Buff.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndBuffDelegate);

UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff : public AAbility_Buff_Master
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();

	virtual void CastAbility_Implementation() override;
public:
	virtual void ApplyBuff();
	//void ClearBuff();
	//void TickBuff();
	//void ClearDuplicates();
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	class UPlayerAnimInstance* AsPlayerAnimInstance;
	//float BuffLifeSpan = 10;
	//float BuffTickRate =.5;
	//FOnEndBuffDelegate OnEndBuffDelegate;
};
