// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "../NPCAIController.h"
#include "../../PuzzlePlatformsCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	//if (ControllingPawn == nullptr)
	//	return false;

	auto Target = Cast<APuzzlePlatformsCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey));
	//if (Target == nullptr)
	//	return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ANPCAIController::AttackRangeKey));

	return bResult;
}