// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAnimInstance.h"
#include "NPC_Archer.h"
UArcherAnimInstance::UArcherAnimInstance()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ArrowAttackMontageAsset(TEXT(
		"/Game/Animation/Montage/ArcherAttackMontage"
	));
	if (ArrowAttackMontageAsset.Succeeded())
	{
		ArrowAttackMontage = ArrowAttackMontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT(
		"/Game/Animation/Montage/ArcherDeath_Montage"
	));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Impact_Montage(TEXT(
		"/Game/Animation/Montage/Archer_Impact_Montage"
	));
	if (Impact_Montage.Succeeded())
	{
		ImpactMontage = Impact_Montage.Object;
	}
}

void UArcherAnimInstance::PlayArrowAttackMontage()
{
	Montage_Play(ArrowAttackMontage, 1.0);
}

void UArcherAnimInstance::AnimNotify_ArrowShot()
{
	auto ArcherRef = Cast<ANPC_Archer>(GetOwningActor());
	ABCHECK(ArcherRef != nullptr);
	if (ArcherRef->HasAuthority())
	{
		OnArrowShotDelegate.Broadcast();
	}
}

void UArcherAnimInstance::PlayImpactMontage()
{
	Montage_Play(ImpactMontage, 1.0);
}

void UArcherAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0);
}