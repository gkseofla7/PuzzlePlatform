// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindBotPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBTTask_FindBotPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_FindBotPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMomory) override;
};
