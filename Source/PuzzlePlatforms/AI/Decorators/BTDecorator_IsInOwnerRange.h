// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInOwnerRange.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBTDecorator_IsInOwnerRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInOwnerRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
