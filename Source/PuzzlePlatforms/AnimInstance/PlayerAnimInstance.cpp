// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PlayersComponent/PlayersMotionReplicator.h"
UPlayerAnimInstance::UPlayerAnimInstance()
{
	CurrenPawnSpeed = 0.0f;
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_ATTACK_MONTAGE(TEXT(
		"/Game/Animation/Player_Attack_Montage"
	));
	if (SWORD_ATTACK_MONTAGE.Succeeded())
	{
		SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
	}

	IsAttacking = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) 
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{

		CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		CurrenPawnSpeed = Pawn->GetVelocity().Size();
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UPlayerAnimInstance::PlaySwordAttackMontage()
{
	auto Pawn = TryGetPawnOwner();
	if (Pawn ==nullptr)
		return;
	//auto Character = Cast<APuzzlePlatformsCharacter>(Pawn);
	//int tmp = Cast<Character->MotionReplicator->CurrentCombo;

	//UE_LOG(LogTemp, Warning, TEXT("CurrentCombo : %d"), tmp);
	Montage_Play(SwordAttackMontage, 1.0);
	
}

void UPlayerAnimInstance::AnimNotify_AttackHitCheck()
{

	OnAttackHitCheck.Broadcast();
}
void UPlayerAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}
void UPlayerAnimInstance::AnimNotify_EndAttack()
{
	OnAttackEndCheck.Broadcast();
	
}

void UPlayerAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//ABCHECK(Montage_IsPlaying(SwordAttackMontage));
	FName tmp = GetAttackMontageSectionName(NewSection);
	UE_LOG(LogTemp, Warning, TEXT("ComboName : %s"), *(tmp.ToString()));
	Montage_JumpToSection(tmp, SwordAttackMontage);
}

FName UPlayerAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	//ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	//FString output = FString::Printf(TEXT("Attack%d"), Section);
	//
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

