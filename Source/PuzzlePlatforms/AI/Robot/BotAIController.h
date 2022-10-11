// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ABotAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABotAIController();
	void SetPlayerRefKey(AActor* NewPlayerRef);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;

public:
	static const FName PlayerRangeKey;
	static const FName PlayerRefKey;
};
