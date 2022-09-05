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

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	TE_PrimaryWeapon UMETA(DisplayName = "PrimaryWeapon"),
	TE_SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon"),

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
	UFUNCTION(BlueprintCallable)
		void EquipItem(class AObject_Master* Item, bool EquipandHold);
	void SetFPSHudWidget();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;
private:
	void WeaponPrimaryPressed();
	void WeaponPrimaryReleased();

	void WeaponSecondaryPressed();
	void WeaponSecondaryReleased();

	void InteractPressed();

	void WeaponReload();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Everyone_SetMuzzleRotation(FRotator NewRotator);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetMuzzleRotation(FRotator NewRotator);


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
		float Direction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector MissileVelocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* EquippedItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* PrimaryWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* SecondaryWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class UFPSHudWidget>FPSHudClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UFPSHudWidget* HudWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsAiming = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool UseTPSMovement = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ECamInUse CamInUse = ECamInUse::TE_TPCam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsItemEquipped;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsFiring = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsReloading = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanAim = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EWeaponSlot WeaponSlotUse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool DoPickupLinetrace;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AObject_Master* PickupItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* RocketHolderComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* MissileComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UDecalComponent* DecalMissileComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USplineComponent* SplinePathComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* GridSphere;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RocketSpeed = 1000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class USplineMeshComponent*> PointsArray;


};
