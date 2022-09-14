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
