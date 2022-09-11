// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Master.h"
#include "NPCAIController.h"
#include "NPCAnimInstance.h"

// Sets default values
ANPC_Master::ANPC_Master()
{
	PrimaryActorTick.bCanEverTick = true;
	//static ConstructorHelpers::FClassFinder<ANPCAIController> AIControllerBPClass(TEXT("/Game/AI/NPC/NPC_AI_Controller"));
	//if (AIControllerBPClass.Class != NULL)
	//{
	//	AIControllerClass = AIControllerBPClass.Class;
	//}
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM((TEXT("/Game/Animation/BP_AI_Ainm")));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);

	}
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

}

// Called to bind functionality to input
void ANPC_Master::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Master::Attack()
{
	MyAnim->PlaySwordAttackMontage();
}


void ANPC_Master::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
}
