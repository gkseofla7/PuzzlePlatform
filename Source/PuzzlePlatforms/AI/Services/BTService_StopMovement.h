// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StopMovement.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBTService_StopMovement : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_StopMovement();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};
