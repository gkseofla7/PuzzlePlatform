// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "../NPCAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/PawnMovementComponent.h"
UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{

	NodeName = TEXT("SetSpeed");
	Interval = 1.0f;
}
//매 초마다 계속 계싼
//맞으면 그쪽으로 움직임 범위 있음 속도 빠름
void UBTService_ChangeSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACharacter* Monster =Cast<ACharacter>( OwnerComp.GetAIOwner()->GetPawn());
	Monster->GetCharacterMovement()->MaxWalkSpeed = Speed;

}
