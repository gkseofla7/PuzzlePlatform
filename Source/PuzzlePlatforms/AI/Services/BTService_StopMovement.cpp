// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_StopMovement.h"
#include "../NPCAIController.h"
#include "../../PuzzlePlatformsCharacter.h"

#include"BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_StopMovement::UBTService_StopMovement()
{

	NodeName = TEXT("IsAttackRange");
	Interval = 0.2f;
}
//�� �ʸ��� ��� ���
//������ �������� ������ ���� ���� �ӵ� ����
void UBTService_StopMovement::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	bool bResult;
	auto Target = Cast<APuzzlePlatformsCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey));
	if (Target == nullptr)
		return;
	if (ControllingPawn == nullptr)
		return;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ANPCAIController::AttackRangeKey));
	if (bResult == true)
		OwnerComp.GetAIOwner()->StopMovement();

}

