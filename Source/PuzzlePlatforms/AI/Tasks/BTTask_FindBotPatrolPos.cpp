// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindBotPatrolPos.h"

#include "../Robot/BotAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

UBTTask_FindBotPatrolPos::UBTTask_FindBotPatrolPos()
{
	NodeName = TEXT("FindBotPatrolPos");
}

EBTNodeResult::Type UBTTask_FindBotPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;
	ACharacter* PlayerRef = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABotAIController::PlayerRefKey));
	FVector Origin = PlayerRef->GetActorLocation() + PlayerRef->GetActorForwardVector() * 400;
	//앞에 있음
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 200, NextPatrol))
	{

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABotAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;

	}
	return EBTNodeResult::Failed;
}