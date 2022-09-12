// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NPC_Master.h"
#include "PatrolPath.h"
#include "../PuzzlePlatformsCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


//const FName ANPCAIController::PathLoopingKey(TEXT("PathLooping"));
//const FName ANPCAIController::DirectionKey(TEXT("Direction"));
//const FName ANPCAIController::WaitTimeKey(TEXT("WaitTime"));
const FName ANPCAIController::HomePosKey(TEXT("HomePos"));
const FName ANPCAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ANPCAIController::TargetKey(TEXT("Target"));

ANPCAIController::ANPCAIController()
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

//void ANPCAIController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		MyPawn = Cast<ANPC_Master>(InPawn);
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couln't run behavior tree!"));
		}
		else
		{

			//Blackboard->SetValueAsBool(PathLoopingKey, MyPawn->PatrolPath->IsLooping);
			//Blackboard->SetValueAsInt(DirectionKey, 1);
			//Blackboard->SetValueAsFloat(WaitTimeKey, MyPawn->PatrolPath->WaitTime);

		}
	}
}
void ANPCAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

//void ANPCAIController::SetTargetKey(APuzzlePlatformsCharacter* PlayerTarget)
//{
//	GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, PlayerTarget);
//	//NetMulticast_SetTargetKey(PlayerTarget);
//}
//
//void ANPCAIController::NetMulticast_SetTargetKey_Implementation(class APuzzlePlatformsCharacter* PlayerTarget)
//{
//	GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, PlayerTarget);
//}
//
//bool ANPCAIController::NetMulticast_SetTargetKey_Validate(class APuzzlePlatformsCharacter* PlayerTarget)
//{
//	return true;
//}
//
//
//void ANPCAIController::SetHomePosKey(FVector NewPos)
//{
//	NetMulticast_SetHomePosKey(NewPos);
//}
//
//void ANPCAIController::NetMulticast_SetHomePosKey_Implementation(FVector NewPos)
//{
//	GetBlackboardComponent()->SetValueAsVector(ANPCAIController::HomePosKey, NewPos);
//}
//
//bool ANPCAIController::NetMulticast_SetHomePosKey_Validate(FVector NewPos)
//{
//	return true;
//}
//
//void ANPCAIController::SetPatrolPosKey(FVector NewPos)
//{
//	GetBlackboardComponent()->SetValueAsVector(ANPCAIController::PatrolPosKey, NewPos);
//	//NetMulticast_SetPatrolPosKey(NewPos);
//}
//
//void ANPCAIController::NetMulticast_SetPatrolPosKey_Implementation(FVector NewPos)
//{
//	GetBlackboardComponent()->SetValueAsVector(ANPCAIController::PatrolPosKey, NewPos);
//}
//
//bool ANPCAIController::NetMulticast_SetPatrolPosKey_Validate(FVector NewPos)
//{
//	return true;
//}