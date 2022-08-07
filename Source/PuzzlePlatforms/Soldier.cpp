// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"

ASoldier::ASoldier()
{
	//MotionReplicator = CreateDefaultSubobject<UPlayersMotionReplicator>(TEXT("MOTIOREPLICATOR"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIO_ANIM((TEXT("/Game/Animation/Soldier_Anim_BP")));
	if (WARRIO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIO_ANIM.Class);
	}
}