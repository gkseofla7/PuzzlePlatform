// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"

#include "../NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if(ControllingPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if(nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANPCAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1200., NextPatrol))
	{

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANPCAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;

	}
	return EBTNodeResult::Failed;
}