// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsCharacter.h"
#include "Soldier.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECamInUse : uint8
{
	TE_FPCam UMETA(DisplayName = "FPCam"),
	TE_TPCam UMETA(DisplayName = "TPCam"),

};
UCLASS()
class PUZZLEPLATFORMS_API ASoldier : public APuzzlePlatformsCharacter
{
	GENERATED_BODY()
public:
	ASoldier();

	UFUNCTION(BlueprintCallable)
	void SetMuzzleRotation();
	UFUNCTION(BlueprintCallable)
		void AimDownSights();
	UFUNCTION(BlueprintCallable)
		void UnAim();
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void WeaponPrimaryPressed();
	void WeaponPrimaryReleased();

	void WeaponSecondaryPressed();
	void WeaponSecondaryReleased();

	void WeaponReload();


private:
	TSubclassOf<class AWeapon_Master>WeaponMasterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPPCam_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* AimObejctFPP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ADSCam_;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* EquippedItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class UFPSHudWidget>FPSHudClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UFPSHudWidget* HudWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsAiming = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool UseTPSMovement = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ECamInUse CamInUse = ECamInUse::TE_FPCam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsItemEquipped = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsFiring = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsReloading = false;
};
