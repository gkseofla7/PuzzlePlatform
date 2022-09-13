// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Archer.h"
#include "NPCAnimInstance.h"
#include "MonsterStatComponent.h"
#include "EnumMonsterType.h"
#include "NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
ANPC_Archer::ANPC_Archer()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//static ConstructorHelpers::FClassFinder<ANPCAIController> AIControllerBPClass(TEXT("/Game/AI/NPC/NPC_AI_Controller"));
	//if (AIControllerBPClass.Class != NULL)
	//{
	//	AIControllerClass = AIControllerBPClass.Class;
	//}
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM((TEXT("/Game/Animation/BP_GoblinAnim")));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);

	}

}

void ANPC_Archer::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast< UNPCAnimInstance>(GetMesh()->GetAnimInstance());
	MonsterStat->CustomInitializeComponent(EMonsterEnum::TE_Archer);
	auto AIController = Cast< ANPCAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsFloat(ANPCAIController::AttackRangeKey, AttackRange);
}

void ANPC_Archer::Attack()
{
	NetMulticast_Attack();
}


void ANPC_Archer::NetMulticast_Attack_Implementation()
{
	//MyAnim->PlaySwordAttackMontage();
}

bool ANPC_Archer::NetMulticast_Attack_Validate()
{
	return true;
}