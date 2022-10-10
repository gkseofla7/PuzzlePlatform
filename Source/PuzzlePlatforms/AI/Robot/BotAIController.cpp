// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ABotAIController::ABotAIController()
{

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/Bot/Bot_BB"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/Bot/Bot_BT"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ABotAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTAsset);
}