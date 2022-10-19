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


}

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

	}
}

void ANPCAIController::SetHomePosKey(FVector NewPos)
{
	GetBlackboardComponent()->SetValueAsVector(ANPCAIController::HomePosKey, NewPos);
}

void ANPCAIController::SetTargetKey(ACharacter_Master* PlayerTarget)
{
	GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, PlayerTarget);
}

void ANPCAIController::SetIsHitKeyTrue()
{
	SetIsHitKey_Implementation(true);
	FTimerHandle TimerHandler;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SetIsHitKey_Implementation"), false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDel, 5.f, false);
}

void ANPCAIController::SetIsHitKey_Implementation(bool NewIsHit)
{
	GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsHitKey, NewIsHit);
}


void ANPCAIController::SetIsAttackingKey(bool NewIsAttackingKey)
{
	GetBlackboardComponent()->SetValueAsBool(ANPCAIController::IsAttackingKey, NewIsAttackingKey);
}

void ANPCAIController::PauseLogic()
{
	BrainComponent->PauseLogic(TEXT("Impacted"));
}

void ANPCAIController::ResumeLogic()
{
	BrainComponent->ResumeLogic(TEXT("Impacted"));
}