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

UENUM(BlueprintType)
enum class EGunType : uint8
{
	TE_Rifle UMETA(DisplayName = "Rifle"),
	TE_Pistol UMETA(DisplayName = "Pistol"),
	TE_Shotgun UMETA(DisplayName = "Shotgun"),
};


UCLASS()
class PUZZLEPLATFORMS_API AWeapon_Master : public AObject_Master
{
	GENERATED_BODY()

public:

	virtual void Tick(float Deltatime);
	AWeapon_Master();
	//void CustomInitialize(class ASoldier* NewPlayer);
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
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SetClipAmmo(float NewClipAmmo);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SetMuzzleRotation();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_AmmoCheck();


	void SetMuzzleRotation(FRotator NewRotator) { MuzzleRotation_ = NewRotator; }


	class UCameraComponent* GetCamera() { return Camera_; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireMode FireMode;

	UPROPERTY(Replicated)
		FRotator MuzzleRotation_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanFire = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TestPhysics = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ReloadDelay = 1.784;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGunType WeaponType = EGunType::TE_Rifle;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool SteamPack = false;
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* MuzzlesParticle;




private:

	TSubclassOf<class ABulletMaster>BulletMasterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera_;

	//class ASoldier* PlayerRef;

};
