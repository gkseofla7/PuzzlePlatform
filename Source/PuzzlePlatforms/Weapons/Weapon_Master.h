// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object_Master.h"
#include "Weapon_Master.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFireMode: uint8 
{
	TE_Single UMETA(DisplayName = "Single"),
	TE_Burst UMETA(DisplayName = "Burst"),
	TE_Auto UMETA(DisplayName = "Auto"),
};

UCLASS()
class PUZZLEPLATFORMS_API AWeapon_Master : public AObject_Master
{
	GENERATED_BODY()

public:

	AWeapon_Master();
	UFUNCTION(BlueprintCallable)
	void FireModeSwitch();
	UFUNCTION(BlueprintCallable)
		void AmmoCheck();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void StartFire();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void StopFire();

	UFUNCTION(BlueprintCallable)
		void Shot();

	UFUNCTION(BlueprintCallable)
		void Reload();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireMode FireMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator MuzzleRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanFire = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ReloadDelay = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		bool CanReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		bool UseRemainingAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		bool HasEnoughAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		bool ClipEmpty;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int ClipAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int BagAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int MaxClipAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int MaxBagAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int AmmoCost = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int AmmoNeeded;



private:

	TSubclassOf<class ABulletMaster>BulletMasterClass;
};
