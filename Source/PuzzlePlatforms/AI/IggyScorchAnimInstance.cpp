// Fill out your copyright notice in the Description page of Project Settings.


#include "IggyScorchAnimInstance.h"
#include "NPC_Boss.h"

UIggyScorchAnimInstance::UIggyScorchAnimInstance()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageAsset(TEXT(
		"/Game/Animation/IggyScorch/Montagne/IggyScorchAttackMontage"
	));
	if (AttackMontageAsset.Succeeded())
	{
		AttackMontage = AttackMontageAsset.Object;
	}


}

void UIggyScorchAnimInstance::UpdateAnimation(float DeltaSeconds, bool bNeedsValidRootMotion, EUpdateAnimationFlag UpdateFlag)
{
	Super::UpdateAnimation(DeltaSeconds, bNeedsValidRootMotion, UpdateFlag);
	UE_LOG(LogTemp, Warning, TEXT("Speed"), Speed);
}

void UIggyScorchAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0);
}

void UIggyScorchAnimInstance::AnimNotify_Shot()
{
	auto BossRef = Cast<ANPC_Boss>(GetOwningActor());
	ABCHECK(BossRef != nullptr);
	if (BossRef->HasAuthority())
	{
		OnShotDelegate.Broadcast();
	}
}


