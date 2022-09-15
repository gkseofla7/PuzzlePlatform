// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "../NPC_Master.h"
#include "../NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
//#include "../GoblinAnimInstance.h"
//#include "../../PuzzlePlatformsCharacter.h"
UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMomory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMomory);
	
	auto Monster = Cast<ANPC_Master>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;

	Monster->Attack();
	IsAttacking = true;
	Monster->OnAttackEnd.AddLambda([this]()-> void {
		UE_LOG(LogTemp, Warning, TEXT("FINISH"));
		IsAttacking = false;
		});
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{//때리면 되돌림

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsHitKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}