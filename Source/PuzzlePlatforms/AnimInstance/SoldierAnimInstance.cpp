// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAnimInstance.h"
#include "../Soldier.h"

#include "../Character_Master.h"
USoldierAnimInstance::USoldierAnimInstance()
{
	CurrenPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT(
		"/Game/Animation/Soldier/Montage/SoldierDeath_Montage"
	));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();

	if (::IsValid(Pawn))
	{
		Player = Cast<ASoldier>(Pawn);
		if (Player)
		{
			IsInAir = Player->GetMovementComponent()->IsFalling();

		}

		IsItemEquipped = Player->IsItemEquipped;
		if (Player->EquippedItem != nullptr)
		{
			WeaponType = Player->EquippedItem->WeaponType;
		}
		IsReloading = Player->IsReloading;


		CurrenPawnSpeed = Player->GetVelocity().Size();
		Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		AimRotation = Player->ControlRotation;
		//if ((Player->IsLocallyControlled() && Player->IsPlayerControlled())||(Player->HasAuthority()&&Player->IsPlayerControlled()))
		//{
		//	AimRotation = Player->ControlRotation;
		//}
		//else// 그 외애들은
		//{

		//	ClientTimeSinceUpdate += DeltaSeconds;
		//	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;

		//	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

		//	if (LerpRatio > 1)
		//		LerpRatio = 1;

		//	AimRotation = FQuat::Slerp(StartControllerRotator.Quaternion(), Player->ControlRotation.Quaternion(), LerpRatio).Rotator();
		//}

	}
}

void USoldierAnimInstance::PlayDeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayDeathMontage"));
	Montage_Play(DeathMontage, 1.0);
}


