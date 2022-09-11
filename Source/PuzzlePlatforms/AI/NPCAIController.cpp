// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NPC_Master.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ANPCAIController::ANPCAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/NPC/NPC_BB"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/NPC/NPC_BT"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couln't run behavior tree!"));
		}
		else
		{
			MyPawn = Cast<ANPC_Master>(InPawn);
		}
	}
}
void ANPCAIController::OnUnPossess()
{
	Super::OnUnPossess();
}