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
	//IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMomory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMomory);
	auto Controller = Cast< ANPCAIController>(OwnerComp.GetAIOwner());
	auto Monster = Cast<ANPC_Master>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Warning, TEXT("StartIsAttacking "));
	Controller->SetIsAttackingKey(true);
	Monster->Attack();
	//IsAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsAttackingKey, true);
	FString tmp = Monster->GetName();
	Monster->OnAttackEnd.Clear();//���ʿ� ���⼭ �θ��� �ȵǳ�,,����
	Monster->OnAttackEnd.AddLambda([Controller]()-> void {
		//��� �����;;
		UE_LOG(LogTemp, Warning, TEXT("FinishIsAttacking %s"), *Controller->GetPawn()->GetName());
		Controller->SetIsAttackingKey(false);
		
		
		});
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	bool IsAttacking = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ANPCAIController::IsAttackingKey);
	//�̰� false �Ǹ�??
	if (!IsAttacking)
	{//������ �ǵ���
		auto Monster = Cast<ANPC_Master>(OwnerComp.GetAIOwner()->GetPawn());

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsHitKey, false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}