// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Boss.h"
#include "MonsterStatComponent.h"
#include "NPCBossAIController.h"
#include "../Character_Master.h"
#include "../Weapons/ScorchBomb.h"
#include "../Weapons/Meteor.h"

#include "IggyScorchAnimInstance.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	ConstructorHelpers::FClassFinder<AMeteor> MeteorBPClass(TEXT("/Game/Weapons/Projectiles/BP_Meteor"));
	if (!ensure(MeteorBPClass.Class != nullptr)) return;
	MeteorClass = MeteorBPClass.Class;
}

void ANPC_Boss::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast< UIggyScorchAnimInstance>(GetMesh()->GetAnimInstance());
	if (HasAuthority()&&MyAnim != nullptr)
	{

		//MyAnim->OnAttackHitCheck.AddUObject(this, &ANPC_Goblin::AttackCheck);
		MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Boss::EndAnimation);
		
		MyAnim->OnShotDelegate.AddUObject(this, &ANPC_Boss::Shot);
		MyAnim->OnFireMeteorDelegate.AddUObject(this, &ANPC_Boss::Meteor);

	}

	if (MonsterStat != nullptr)
	{
		MonsterStat->CustomInitializeComponent(EMonsterEnum::TE_IggyScorch);
	}

	if (HasAuthority())
	{
		auto AIController = Cast< ANPCBossAIController>(GetController());
		AIController->GetBlackboardComponent()->SetValueAsFloat(ANPCBossAIController::AttackRangeKey, AttackRange);

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

void ANPC_Boss::Meteor()
{
	int n = 5;//총 5번만~
	float t = 5.f;
	float g = -9.81f;//m/s기준이니
	g = g * 100;//cm/s 로 바꿔줌
	FVector StartLocation = GetMesh()->GetSocketLocation("FX_FlameBreath");
	//TArray<AActor*> Players;

	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;//
	UClass* ActorClassFilter = AActor::StaticClass();
	TArray<AActor*> ActorsToIgnore;// TArray<AActor*>& OutActors)
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);//아마 이게 Character
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//아마 이게 Pawn
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 1000., ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);
	UE_LOG(LogTemp, Warning, TEXT("OverlapActors : %d"), OutActors.Num());
	for (int i = 0; i < OutActors.Num(); i++)
	{
		if (Cast<ACharacter_Master>(OutActors[i]) == nullptr)
			continue;
		float Vx = (OutActors[i]->GetActorLocation().X - StartLocation.X) / t;
		float Vy = (OutActors[i]->GetActorLocation().Y - StartLocation.Y) / t;
		float Vz = (OutActors[i]->GetActorLocation().Z - StartLocation.Z - g * t * t) / t;
		UE_LOG(LogTemp, Warning, TEXT("Velocity : %f, %f, %f"), Vx, Vy, Vz);
		FVector MeteorVelocity = FVector(Vx, Vy, Vz);

		FTransform MeteorTransform;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = Cast<AActor>(this);
		MeteorTransform.SetLocation(GetMesh()->GetSocketTransform("FX_FlameBreath").GetLocation());

		auto DirRot = UKismetMathLibrary::MakeRotFromX(MeteorVelocity);
		MeteorTransform.SetRotation(GetMesh()->GetSocketTransform("FX_FlameBreath").GetRotation());

		//ArrowTransform.SetScale3D(BulletScale);
		AActor* Meteor = GetWorld()->SpawnActor<AMeteor>(MeteorClass, MeteorTransform, SpawnInfo);
		if (Meteor == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Meteor Nullptr"));
			return;
		}
		AMeteor* MyMeteor = Cast<AMeteor>(Meteor);
		MyMeteor->ProjectileComponent->Velocity = MeteorVelocity;
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
	//여기서 확률
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
	if (Montage == MyAnim->AttackMontage || Montage == MyAnim->FireBlastMontage||Montage == MyAnim->MeteorMontage)
	{
		MyAnim->FullBody = true;
		OnAttackEnd.Broadcast();
	}


}