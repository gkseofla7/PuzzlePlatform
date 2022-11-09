// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
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


	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//void NetMulticast_SetTargetKey(class ACharacter_Master* PlayerTarget);
	void SetTargetKey(class ACharacter_Master* PlayerTarget);
	void SetHomePosKey(FVector NewPos);

	void SetbIsAttackingKey(bool NewbIsAttackingKey);
	void SetIsHitKeyTrue();

	void PauseLogic();
	void ResumeLogic();
	//UFUNCTION(NetMulticast, Reliable, WithValidation)
	//	void NetMulticast_SetHomePosKey(FVector NewPos);
private:
	UFUNCTION()
	void SetIsHitKey_Implementation(bool NewIsHit);

protected:
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
	static const FName TargetLocationKey;
	static const FName AttackRangeKey;
	static const FName IsHitKey;
	static const FName bIsAttackingKey;
	static const FName IsAttackedKey;
	int i = 0;

};
