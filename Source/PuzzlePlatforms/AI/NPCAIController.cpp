// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "NPC_Master.h"
#include "PatrolPath.h"
#include "../Character_Master.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


//const FName ANPCAIController::PathLoopingKey(TEXT("PathLooping"));
//const FName ANPCAIController::DirectionKey(TEXT("Direction"));
//const FName ANPCAIController::WaitTimeKey(TEXT("WaitTime"));
const FName ANPCAIController::HomePosKey(TEXT("HomePos"));
const FName ANPCAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ANPCAIController::TargetKey(TEXT("Target"));
const FName ANPCAIController::AttackRangeKey(TEXT("AttackRange"));
const FName ANPCAIController::IsHitKey(TEXT("IsHit"));
const FName ANPCAIController::IsAttackingKey(TEXT("IsAttacking"));

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
		Blackboard->SetValueAsBool(ANPCAIController::IsAttackingKey, true);
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
void ANPCAIController::SetHomePosKey(FVector NewPos)
{
	GetBlackboardComponent()->SetValueAsVector(ANPCAIController::HomePosKey, NewPos);
}

void ANPCAIController::SetTargetKey(ACharacter_Master* PlayerTarget)
{
	GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, PlayerTarget);
}

void ANPCAIController::SetIsHitKey(bool NewIsHit)
{
	GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsHitKey, NewIsHit);
}


void ANPCAIController::SetIsAttackingKey(bool NewIsAttackingKey)
{
	GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsAttackingKey, NewIsAttackingKey);
}