// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABotAIController::PlayerRefKey(TEXT("PlayerRef"));
const FName ABotAIController::PlayerRangeKey(TEXT("PlayerRange"));
const FName ABotAIController::PatrolPosKey(TEXT("PatrolPos"));
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
	UE_LOG(LogTemp, Warning, TEXT("RunBehaviorTree"));
	RunBehaviorTree(BTAsset);
}

void ABotAIController::SetPlayerRefKey(AActor* NewPlayerRef)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *NewPlayerRef->GetName());
	GetBlackboardComponent()->SetValueAsObject(ABotAIController::PlayerRefKey, NewPlayerRef);
}