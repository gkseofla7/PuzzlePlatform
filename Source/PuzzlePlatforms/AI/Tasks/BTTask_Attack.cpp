// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "../NPC_Master.h"
#include "../NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
//#include "../GoblinAnimInstance.h"
//#include "../../Character_Master.h"
UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	//bIsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMomory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMomory);
	auto Controller = Cast< ANPCAIController>(OwnerComp.GetAIOwner());
	auto Monster = Cast<ANPC_Master>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Warning, TEXT("StartbIsAttacking "));
	Controller->SetbIsAttackingKey(true);
	Monster->Attack();
	//bIsAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::bIsAttackingKey, true);
	FString tmp = Monster->GetName();
	Monster->OnAttackEnd.Clear();//애초에 여기서 부르면 안되네,,ㅋㅋ
	Monster->OnAttackEnd.AddLambda([Controller]()-> void {
		//계속 실행됨;;
		UE_LOG(LogTemp, Warning, TEXT("FinishbIsAttacking %s"), *Controller->GetPawn()->GetName());
		Controller->SetbIsAttackingKey(false);
		
		
		});
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	bool bIsAttacking = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ANPCAIController::bIsAttackingKey);
	//이게 false 되면??
	if (!bIsAttacking)
	{//때리면 되돌림
		auto Monster = Cast<ANPC_Master>(OwnerComp.GetAIOwner()->GetPawn());

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsHitKey, false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}