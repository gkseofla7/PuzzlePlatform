// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBTService_ChangeSpeed : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_ChangeSpeed();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed = 550;
};
