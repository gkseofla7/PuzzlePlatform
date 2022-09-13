// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAnimInstance.h"


UNPCAnimInstance::UNPCAnimInstance()
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

void UNPCAnimInstance::PlaySwordAttackMontage()
{
	Montage_Play(SwordAttackMontage, 1.0);
}

void UNPCAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}