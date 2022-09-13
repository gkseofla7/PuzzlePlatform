// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Master.h"
#include "NPCAIController.h"
#include "NPCAnimInstance.h"
#include "MonsterStatComponent.h"

#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANPC_Master::ANPC_Master()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//static ConstructorHelpers::FClassFinder<ANPCAIController> AIControllerBPClass(TEXT("/Game/AI/NPC/NPC_AI_Controller"));
	//if (AIControllerBPClass.Class != NULL)
	//{
	//	AIControllerClass = AIControllerBPClass.Class;
	//}
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	MonsterStat = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStat"));
}

// Called when the game starts or when spawned
void ANPC_Master::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast< UNPCAnimInstance>(GetMesh()->GetAnimInstance());
	MyAnim->OnMontageEnded.AddDynamic(this, &ANPC_Master::EndAnimation);

}

// Called every frame
void ANPC_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetSpeed(GetVelocity().Size());

}

// Called to bind functionality to input
void ANPC_Master::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Master::Attack()
{
	//NetMulticast_Attack();
}






void ANPC_Master::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
}


float ANPC_Master::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
		return 0;

	//ABCHECK(MotionReplicator != nullptr)

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MonsterStat->IncreaseHP(-FinalDamage);
	return FinalDamage;
}


