// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Boss.h"
#include "MonsterStatComponent.h"
#include "NPCBossAIController.h"
#include "../Character_Master.h"
#include "../Weapons/ScorchBomb.h"

#include "IggyScorchAnimInstance.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
ANPC_Boss::ANPC_Boss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Game/ParagonIggyScorch/Characters/Heroes/IggyScorch/Meshes/IggyScorch"));
	USkeletalMesh* Asset = MeshAsset.Object;
	GetMesh()->SetSkeletalMesh(Asset);
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(GetMesh(),"FX_FlameBreath");
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM((TEXT("/Game/Animation/IggyScorch/IggyScorch_AnimBP")));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);
	}
	AIControllerClass = ANPCBossAIController::StaticClass();
	bReplicates = true;
	ConstructorHelpers::FClassFinder<AScorchBomb> ScorchBombBPClass(TEXT("/Game/Weapons/Projectiles/BP_ScorchBomb"));
	if (!ensure(ScorchBombBPClass.Class != nullptr)) return;
	ScorchBombClass = ScorchBombBPClass.Class;
}

void ANPC_Boss::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast< UIggyScorchAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnim != nullptr)
	{

		//MyAnim->OnAttackHitCheck.AddUObject(this, &ANPC_Goblin::AttackCheck);
		MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Boss::EndAnimation);

	}

	if (MonsterStat != nullptr)
	{
		MonsterStat->CustomInitializeComponent(EMonsterEnum::TE_IggyScorch);
	}

	if (HasAuthority())
	{
		auto AIController = Cast< ANPCBossAIController>(GetController());
		AIController->GetBlackboardComponent()->SetValueAsFloat(ANPCBossAIController::AttackRangeKey, AttackRange);
		MyAnim->OnShotDelegate.AddUObject(this, &ANPC_Boss::Shot);
	}

}

void ANPC_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC_Boss::Shot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shot"));
	if (Target != nullptr)
	{
		//Multicast_SetMuzzleRotation();
		FVector BombScale;
		// BulletScale.Set(0.1, 0.1, 0.1);
		FTransform BombTransform;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = Cast<AActor>(this);
		FVector Dir = Target->GetActorLocation() - GetMesh()->GetSocketTransform("Goblin_hand_rSocket").GetLocation();
		BombTransform.SetLocation(GetMesh()->GetSocketTransform("Goblin_hand_rSocket").GetLocation());
		auto DirRot = UKismetMathLibrary::MakeRotFromX(Dir);
		BombTransform.SetRotation(DirRot.Quaternion());

		//ArrowTransform.SetScale3D(BulletScale);
		GetWorld()->SpawnActor<AScorchBomb>(ScorchBombClass, BombTransform,SpawnInfo );

	}
}

void ANPC_Boss::ActivateParticle(bool NewActivate)
{
	if (NewActivate == true)
	{
		ParticleComponent->Activate(true);
	}
	else
	{
		ParticleComponent->Deactivate();
	}

}



float ANPC_Boss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return 0.;
}

void ANPC_Boss::Attack()
{
	NetMulticast_Attack();
}

void ANPC_Boss::NetMulticast_Attack_Implementation()
{
	//¿©±â¼­ È®·ü
	//MyAnim->PlayFireBlastMontage();
	MyAnim->PlayMateorMontage();
	//MyAnim->PlayAttackMontage();
}

bool ANPC_Boss::NetMulticast_Attack_Validate()
{
	return true;
}

void ANPC_Boss::ChangeDamageColor()
{

}

void ANPC_Boss::ChangeOriginalColor()
{

}

void ANPC_Boss::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	MyAnim->bMateor = false;
	if (Montage == MyAnim->AttackMontage || Montage == MyAnim->FireBlastMontage)
	{
		MyAnim->FullBody = true;
		OnAttackEnd.Broadcast();
	}


}