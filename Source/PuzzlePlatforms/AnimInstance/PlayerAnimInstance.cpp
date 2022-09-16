// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PlayersComponent/PlayersMotionReplicator.h"
#include "../Warrior.h"
UPlayerAnimInstance::UPlayerAnimInstance()
{
	CurrenPawnSpeed = 0.0f;
	IsInAir = false;
#pragma region MontageAssetsSetting
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_ATTACK_MONTAGE(TEXT(
		"/Game/Animation/Player_Attack_Montage"
	));
	if (SWORD_ATTACK_MONTAGE.Succeeded())
	{
		SwordAttackMontage = SWORD_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIREBALLATTACKMONTAGE(TEXT(
		"/Game/Animation/Montage/Fireball_Montage"
	));
	if (FIREBALLATTACKMONTAGE.Succeeded())
	{
		FireballAttackMontage = FIREBALLATTACKMONTAGE.Object;
	}



	static ConstructorHelpers::FObjectFinder<UAnimMontage> HANG_TO_CROUCH_MONTAGE(TEXT(
		"/Game/Animation/BlendSpace/Hang_to_Crouch_fixed_Montage"
	));
	if (HANG_TO_CROUCH_MONTAGE.Succeeded())
	{
		HangToCrouchMontage = HANG_TO_CROUCH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AREA_ATTACK_MONTAGE(TEXT(
		"/Game/Animation/Montage/AoE_Montage"
	));
	if (AREA_ATTACK_MONTAGE.Succeeded())
	{
		AreaAttackMontage = AREA_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_BLAST_MONTAGE(TEXT(
		"/Game/Animation/Montage/SwordSlash_Montage"
	));
	if (SWORD_BLAST_MONTAGE.Succeeded())
	{
		SwordBlastMontage = SWORD_BLAST_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DASHMONTAGE(TEXT(
		"/Game/Animation/Montage/Dash_Montage"
	));
	if (DASHMONTAGE.Succeeded())
	{
		DashMontage = DASHMONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT(
		"/Game/Animation/Montage/WarriorDeath_Montage"
	));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}

#pragma endregion MontageAssetsSetting
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
		auto Warrior = Cast<AWarrior>(Pawn);
		if (Warrior != nullptr)
		{
			IsClimbing = Warrior->IsClimbing;
			MoveUpDown = 100*Warrior->MoveUpDown;
			MoveRightLeft =100* Warrior->MoveRightLeft;
		}
	}
}

void UPlayerAnimInstance::PlaySwordAttackMontage()
{
	Montage_Play(SwordAttackMontage, 1.0);
}

void UPlayerAnimInstance::PlayFireballAttackMontage()
{

	Montage_Play(FireballAttackMontage, 1.0);

}

void UPlayerAnimInstance::PlayAoEAttackMontage()
{

	Montage_Play(AreaAttackMontage, 1.0);



}
void UPlayerAnimInstance::PlaySwordBlastMontage()
{

	Montage_Play(SwordBlastMontage, 1.0);
	UE_LOG(LogTemp, Warning, TEXT("PlaySwordMontage"));
}
void UPlayerAnimInstance::PlayDashMontage()
{
	Montage_Play(DashMontage, 2.0);
}

void UPlayerAnimInstance::PlayHangToCrouchMontage()
{
	Montage_Play(HangToCrouchMontage, 1.0);
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
void UPlayerAnimInstance::AnimNotify_LaunchProjectile()
{
	OnFireBall.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Notify!"));
}
void UPlayerAnimInstance::AnimNotify_MovePlace()
{
	OnHangMovePlace.Broadcast();

}
void UPlayerAnimInstance::AnimNotify_ActivateSpell()
{
	OnActivateSpell.Broadcast();
}
void UPlayerAnimInstance::AnimNotify_FireDragonBlast()
{
	OnFireDragonBlastDelegate.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("NotifyOn"));
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



void UPlayerAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0);
}
