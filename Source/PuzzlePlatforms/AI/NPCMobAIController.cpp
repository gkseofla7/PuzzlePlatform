// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCMobAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ANPCMobAIController::ANPCMobAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/GeneralNPC/BB_GeneralNPC"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/GeneralNPC/BT_GeneralNPC"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}