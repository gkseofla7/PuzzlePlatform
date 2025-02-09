// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Goblin.h"
#include "GoblinAnimInstance.h"
#include "MonsterStatComponent.h"
#include "EnumMonsterType.h"
#include "NPCMobAIController.h"

#include "DrawDebugHelpers.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Net/UnrealNetwork.h"
ANPC_Goblin::ANPC_Goblin()
	:Super()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM((TEXT("/Game/Animation/BP_GoblinAnim")));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ImpactedGoblinMaterialAsset(TEXT("/Game/Animation/GoblinAssets/phong1_Inst"));
	ImpactedGoblinMaterial = ImpactedGoblinMaterialAsset.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GoblinMaterialAsset(TEXT("/Game/Animation/GoblinAssets/phong1"));
	GoblinMaterial = GoblinMaterialAsset.Object;

}

void ANPC_Goblin::BeginPlay()

{
	Super::BeginPlay();
	MyAnim = Cast< UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnim != nullptr)
	{

		MyAnim->OnAttackHitCheck.AddUObject(this, &ANPC_Goblin::AttackCheck);
		MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Goblin::EndAnimation);

	}
	if (MonsterStat != nullptr)
	{
		MonsterStat->CustomInitializeComponent(EMonsterEnum::TE_Goblin);
	}



	if (HasAuthority())
	{
		auto AIController = Cast< ANPCMobAIController>(GetController());
		AIController->GetBlackboardComponent()->SetValueAsFloat(ANPCMobAIController::AttackRangeKey, AttackRange);
	}
}

void ANPC_Goblin::Attack()
{
	NetMulticast_Attack();
}

void ANPC_Goblin::AttackCheck()
{

	if (HasAuthority())
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		float AttackRadius = 50.f;


		bool bResult = GetWorld()->SweepSingleByChannel(
			OUT HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * AttackRange,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(AttackRadius),
			Params);
		FVector Vec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + Vec * 0.5f;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
		FColor DrawColor;
		if (bResult)
			DrawColor = FColor::Green;
		else
			DrawColor = FColor::Red;


		//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
			//Rotation, DrawColor, false, 5.f);
		if (bResult && HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), MonsterStat->AttackDamage);
			HitResult.Actor->TakeDamage(MonsterStat->AttackDamage, DamageEvent, GetController(), this);
			//UAISense_Damage::ReportDamageEvent(GetWorld(), HitResult.Actor.Get(), this, 10., HitResult.TraceStart, HitResult.Location);
		}
	}
}

void ANPC_Goblin::ChangeDamageColor()
{
	GetMesh()->SetMaterial(0, ImpactedGoblinMaterial);
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ANPC_Goblin::ChangeOriginalColor, 1., false);
}

void ANPC_Goblin::ChangeOriginalColor()
{
	GetMesh()->SetMaterial(0, GoblinMaterial);
}

void ANPC_Goblin::NetMulticast_Attack_Implementation()
{
	MyAnim->PlaySwordAttackMontage();
}

bool ANPC_Goblin::NetMulticast_Attack_Validate()
{
	return true;
}

void ANPC_Goblin::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MyAnim->SwordAttackMontage)
	{
		OnAttackEnd.Broadcast();
	}
	
	if (HasAuthority()&&Montage == MyAnim->ImpactMontage)
	{
		auto AIController = Cast< ANPCMobAIController>(GetController());
		AIController->ResumeLogic();
	}

}

void ANPC_Goblin::PlayImpactMontage()
{
	MyAnim->PlayImpactMontage();

}



void ANPC_Goblin::Die()
{
	Super::Die();
	MyAnim->PlayDeathMontage();
	UE_LOG(LogTemp, Warning, TEXT("PlayDieMontage"));
	//SetActorEnableCollision(false);

}