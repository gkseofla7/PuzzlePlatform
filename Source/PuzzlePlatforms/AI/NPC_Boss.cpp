// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Boss.h"
#include "MonsterStatComponent.h"
#include "NPCBossAIController.h"
#include "../Character_Master.h"
#include "../Weapons/ScorchBomb.h"
#include "../Weapons/Meteor.h"
#include "../Weapons/Meteor_Target.h"
#include "Kismet/KismetMathLibrary.h"
#include "../AbilitySystem/Ability.h"

#include "IggyScorchAnimInstance.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"
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

	static ConstructorHelpers::FClassFinder<AAbility> Ability_Buff_Fire_BPClass(TEXT("/Game/AbilitySystem/Buff/BP_Ability_Buff_Fire"));
	if (Ability_Buff_Fire_BPClass.Succeeded())
	{
		Ability_Buff_Fire_Class = Ability_Buff_Fire_BPClass.Class;

		//EquippedItem->Get
	}


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
		MyAnim->OnFireMeteorDelegate.AddUObject(this, &ANPC_Boss::MeteorTarget);
		MyAnim->OnFireRandomMeteorDelegate.AddUObject(this, &ANPC_Boss::MeteorRandom);

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
	if (HasAuthority() != true)
		return;
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

void ANPC_Boss::MeteorRandom()
{
	if (HasAuthority() != true)
		return;
	FVector RandomLocation = Target->GetActorLocation();
	UKismetMathLibrary::RandomInteger(150);
	RandomLocation.X += UKismetMathLibrary::RandomInteger(300)-150.;
	RandomLocation.Y += UKismetMathLibrary::RandomInteger(300) - 150.;
	RandomLocation.Z -= 90.f;

	Meteor(RandomLocation);
}

void ANPC_Boss::MeteorTarget()
{
	if (HasAuthority() != true)
		return;
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z -= 90.f;
	Meteor(TargetLocation);
}

void ANPC_Boss::Meteor(FVector MeteorLocation)
{
	float t = 3.f;
	float g = 9.81f;//m/s기준이니
	g = g * 100;//cm/s 로 바꿔줌
	FVector StartLocation = GetMesh()->GetSocketLocation("FX_FlameBreath");
	//TArray<AActor*> Players;

	float Vx = (MeteorLocation.X - StartLocation.X) / t;
	float Vy = (MeteorLocation.Y - StartLocation.Y) / t;
	float Vz = (MeteorLocation.Z - StartLocation.Z + g * t * t / 2) / t;
	FVector MeteorVelocity = FVector(Vx, Vy, Vz);

	//Meteor(MeteorVelocity, MeteorLocation);
	FTransform MeteorTransform;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = Cast<AActor>(this);
	MeteorTransform.SetLocation(GetMesh()->GetSocketTransform("FX_FlameBreath").GetLocation());
	MeteorTransform.SetRotation(GetMesh()->GetSocketTransform("FX_FlameBreath").GetRotation());
	FTransform MeteorTargetTransform;

	//ArrowTransform.SetScale3D(BulletScale);
	AActor* Meteor = GetWorld()->SpawnActor<AMeteor>(MeteorClass, MeteorTransform, SpawnInfo);

	AMeteor* MyMeteor = Cast<AMeteor>(Meteor);
	MyMeteor->NetMulticast_SetSpeed(MeteorVelocity);
	MyMeteor->NetMulticast_SetDecalLocation(MeteorLocation);

	//TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;//
	//UClass* ActorClassFilter = AActor::StaticClass();
	//TArray<AActor*> ActorsToIgnore;// TArray<AActor*>& OutActors)
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);//아마 이게 Character
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//아마 이게 Pawn
	//TArray<AActor*> OutActors;
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 1000., ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);
	//UE_LOG(LogTemp, Warning, TEXT("OverlapActors : %d"), OutActors.Num());
	//for (int i = 0; i < OutActors.Num(); i++)
	//{
	//	if (Cast<ACharacter_Master>(OutActors[i]) == nullptr)
	//		continue;


	//}
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
	float RandomValue =UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
	NetMulticast_Attack(RandomValue);
}

void ANPC_Boss::NetMulticast_Attack_Implementation(float RandomValue)
{
	//여기서 확률
	if (RandomValue < .75f)//일반공격
	{
		MyAnim->PlayAttackMontage();
	}
	else if (RandomValue < .9f)
	{
		MyAnim->PlayFireBlastMontage();
	}
	else
	{
		if (HasAuthority())
		{
			auto MyController = Cast< ANPCBossAIController>(GetController());
			MyController->SetStopMovingKey(true);
		}
		MyAnim->PlayMateorMontage();
	}


}

void ANPC_Boss::StartFireBlast()
{
	UE_LOG(LogTemp, Warning, TEXT("FireBlast!"));
	GetWorld()->GetTimerManager().SetTimer(FireBlastTimer, this, &ANPC_Boss::FireBlast, .5, true);
}
void ANPC_Boss::EndFireBlast()
{
	UE_LOG(LogTemp, Warning, TEXT("EndFireBlast!"));
	GetWorld()->GetTimerManager().ClearTimer(FireBlastTimer);
}
void ANPC_Boss::FireBlast()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float lAttackRange = 200.f;
	float AttackRadius = 200.f;
	TArray< struct FHitResult > OutHits;

	bool bResult = GetWorld()->SweepMultiByChannel(
		OutHits,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * lAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult )
	{
		for (int i = 0; i < OutHits.Num(); i++)
		{
			auto Player = Cast<ACharacter_Master>(OutHits[i].Actor);
			if (Player != nullptr)
			{

				FDamageEvent DamageEvent;
				Player->TakeDamage(5.f, DamageEvent, nullptr, this);//계속 영향주게해야됨
				Player->ReplicateComponent->AbilitySpawn(Ability_Buff_Fire_Class);
			}
		

		}
		
	}
}

bool ANPC_Boss::NetMulticast_Attack_Validate(float RandomValue)
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
	if (Montage == MyAnim->MeteorMontage)
	{
		if (HasAuthority())
		{
			auto MyController = Cast< ANPCBossAIController>(GetController());
			MyController->SetStopMovingKey(false);
		}
	}


}