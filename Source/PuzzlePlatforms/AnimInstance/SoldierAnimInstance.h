// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Master.h"
#include "../Weapons/Weapon_Master.h"
#include "SoldierAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API USoldierAnimInstance : public UAnimInstance_Master
{
	GENERATED_BODY()

public:
	USoldierAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//이건 Tick마다 호출하는 함수

	void PlayDeathMontage();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrenPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		FRotator AimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsItemEquipped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsReloading = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
		EGunType WeaponType;
	class ASoldier* Player;
public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* DeathMontage;

public:

};
