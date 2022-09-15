// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAnimInstance.h"


UGoblinAnimInstance::UGoblinAnimInstance()
: Super()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_ATTACK_MONTAGE(TEXT(
		"/Game/Animation/Montage/Goblin_Attack_Montage"
	));
	if (SWORD_ATTACK_MONTAGE.Succeeded())
	{
		SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
	}
}

void UGoblinAnimInstance::PlaySwordAttackMontage()
{
	Montage_Play(SwordAttackMontage, 1.0);
}

void UGoblinAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}


void UGoblinAnimInstance::AnimNotify_EndTest()
{
	UE_LOG(LogTemp, Warning, TEXT("EndTEST"));
	OnEndTestDelegate.Broadcast();
}