// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ANPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPCAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	//void SetTargetKey(class APuzzlePlatformsCharacter* PlayerTarget);
	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//void NetMulticast_SetTargetKey(class APuzzlePlatformsCharacter* PlayerTarget);

	//void SetHomePosKey(FVector NewPos);
	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//	void NetMulticast_SetHomePosKey(FVector NewPos);



private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ANPC_Master* MyPawn;

	//static const FName PathLoopingKey;
	//static const FName DirectionKey;
	//static const FName WaitTimeKey;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	int i = 0;

};
