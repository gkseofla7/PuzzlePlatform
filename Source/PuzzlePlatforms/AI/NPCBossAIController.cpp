// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBossAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
const FName ANPCBossAIController::StopMovingKey(TEXT("StopMoving"));
ANPCBossAIController::ANPCBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BossNPC/BB_BossNPC"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BossNPC/BT_BossNPC"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ANPCBossAIController::SetStopMovingKey(bool NewStopMovingKey)
{
	GetBlackboardComponent()->SetValueAsBool(ANPCBossAIController::StopMovingKey, NewStopMovingKey);
}
