// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAnimInstance.h"
#include "../Soldier.h"

#include "../PuzzlePlatformsCharacter.h"
USoldierAnimInstance::USoldierAnimInstance()
{
	CurrenPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT(
		"/Game/Animation/Montage/SoldierDeath_Montage"
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
		CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		CurrenPawnSpeed = Pawn->GetVelocity().Size();
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
		FRotator ControlRotation = Pawn->GetControlRotation();
		//float A = 360.0 - ControlRotation.Vector().X;
		float A = 360.0 - ControlRotation.Pitch;
		//float B = ControlRotation.Vector().Y* -1;
		float B = ControlRotation.Pitch;
		//float C = ControlRotation.Vector().Z;
		float C = ControlRotation.Yaw;
		float tmp = 0;
		if (B >= 180)
		{
			tmp = A / 3;
		}
		else
		{
			tmp = B *(-1)/ 3;
		}
		AimRotation = FRotator(0, 0, tmp);
		
		//UE_LOG(LogTemp, Warning, TEXT("%f"), AimRotation.Vector().Y);
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			
		}

		IsItemEquipped = Player->IsItemEquipped;
		if (Player->EquippedItem != nullptr)
		{
			WeaponType = Player->EquippedItem->WeaponType;
		}
		IsReloading = Player->IsReloading;
	}
}

void USoldierAnimInstance::PlayDeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayDeathMontage"));
	Montage_Play(DeathMontage, 1.0);
}
