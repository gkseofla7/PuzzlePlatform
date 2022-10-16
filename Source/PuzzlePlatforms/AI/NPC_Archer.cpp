// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Archer.h"
#include "MonsterStatComponent.h"
#include "EnumMonsterType.h"
#include "NPCAIController.h"
#include "ArcherAnimInstance.h"
#include "../Character_Master.h"
#include "../Weapons/ArrowMaster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BrainComponent.h"
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

	ConstructorHelpers::FClassFinder<AArrowMaster> ArrowMasterBPClass(TEXT("/Game/Weapons/Projectiles/BP_ArrowMaster"));
	if (!ensure(ArrowMasterBPClass.Class != nullptr)) return;
	ArrowMasterClass = ArrowMasterBPClass.Class;
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
	MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Archer::EndAnimation);
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
	if (Target != nullptr)
	{
		//Multicast_SetMuzzleRotation();
		FVector ArrowScale;
		// BulletScale.Set(0.1, 0.1, 0.1);
		FTransform ArrowTransform;
		FVector Dir = Target->GetActorLocation()-GetMesh()->GetSocketTransform("LeftHandSocket").GetLocation();
		ArrowTransform.SetLocation(GetMesh()->GetSocketTransform("LeftHandSocket").GetLocation());
		auto DirRot = UKismetMathLibrary::MakeRotFromX(Dir);
		ArrowTransform.SetRotation(DirRot.Quaternion());
		//ArrowTransform.SetScale3D(BulletScale);
		GetWorld()->SpawnActor<AArrowMaster>(ArrowMasterClass, ArrowTransform);
		
	}
}

void ANPC_Archer::Attack()
{
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

void ANPC_Archer::NetMulticast_SetTarget_Implementation(class ACharacter_Master* NewTarget)
{
	Target = NewTarget;
}

bool ANPC_Archer::NetMulticast_SetTarget_Validate(class ACharacter_Master* NewTarget)
{
	return true;
}

void ANPC_Archer::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MyAnim->ArrowAttackMontage)
	{
		OnAttackEnd.Broadcast();//이새끼가 계속 end때림.. 해도 왜.. Goblin까지 터짐? 아니 무슨 공용이여?
	}
}

void ANPC_Archer::Die()
{
	Super::Die();
	MyAnim->PlayDeathMontage();
	//SetActorEnableCollision(false);

}