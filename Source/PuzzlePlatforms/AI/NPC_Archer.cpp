// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Archer.h"
#include "MonsterStatComponent.h"
#include "EnumMonsterType.h"
#include "NPCAIController.h"
#include "ArcherAnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
ANPC_Archer::ANPC_Archer()
	:Super()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//static ConstructorHelpers::FClassFinder<ANPCAIController> AIControllerBPClass(TEXT("/Game/AI/NPC/NPC_AI_Controller"));
	//if (AIControllerBPClass.Class != NULL)
	//{
	//	AIControllerClass = AIControllerBPClass.Class;
	//}
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM((TEXT("/Game/Animation/BP_ArcherAnim")));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);

	}

}

void ANPC_Archer::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast< UArcherAnimInstance>(GetMesh()->GetAnimInstance());
	MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Master::EndAnimation);
	MonsterStat->CustomInitializeComponent(EMonsterEnum::TE_Archer);
	if (HasAuthority())
	{
		auto AIController = Cast< ANPCAIController>(GetController());
		AIController->GetBlackboardComponent()->SetValueAsFloat(ANPCAIController::AttackRangeKey, AttackRange);
		MyAnim->OnArrowShotDelegate.AddUObject(this, &ANPC_Archer::ArrowShot);
	}

}

void ANPC_Archer::ArrowShot()
{

}

void ANPC_Archer::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	NetMulticast_Attack();
}


void ANPC_Archer::NetMulticast_Attack_Implementation()
{
	MyAnim->PlayArrowAttackMontage();
}

bool ANPC_Archer::NetMulticast_Attack_Validate()
{
	return true;
}