// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInOwnerRange.h"
#include "../Robot/BotAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
UBTDecorator_IsInOwnerRange::UBTDecorator_IsInOwnerRange()
	:Super()
{
	NodeName = TEXT("InOwnerRange");
}


bool UBTDecorator_IsInOwnerRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	//if (ControllingPawn == nullptr)
	//	return false;

	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABotAIController::PlayerRefKey));
	//if (Target == nullptr)
	//	return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ABotAIController::PlayerRangeKey));
	//OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", Target->GetActorLocation());
	//범위 내에 있으면 true~
	//if (bResult == true)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("bResult : True"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("bResult : False"));
	//}

	return bResult;
}