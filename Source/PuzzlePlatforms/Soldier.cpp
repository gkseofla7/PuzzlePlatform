// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "PlayersComponent/SoldierMotionReplicator.h"
#include "AnimInstance/SoldierAnimInstance.h"
ASoldier::ASoldier()
{
	MotionReplicator_ = CreateDefaultSubobject<USoldierMotionReplicator>(TEXT("SoldierMotionReplicator"));

	if (MotionReplicator_ != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Here Is the problem"));
	static ConstructorHelpers::FClassFinder<USoldierAnimInstance> SOLDIER_ANIM((TEXT("/Game/Animation/BP_SoldierAnim")));
	if (SOLDIER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SOLDIER_ANIM.Class);
	}
}

void ASoldier::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<USoldierAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);


}