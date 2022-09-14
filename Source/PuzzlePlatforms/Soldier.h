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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void AddSplineMeshComponent(FVector StartPoint, FVector StartTangent, FVector EndPoint, FVector EndTangent);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ClearPointsArray();
	void SteamPack();
	void UnSteamPack();
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;
	virtual void AddControllerPitchInput(float Val);
private:

	void WeaponPrimaryReleased();
	void WeaponSecondaryPressed();
	void WeaponSecondaryReleased();
	void InteractPressed();
	void WeaponReload();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Everyone_SetMuzzleRotation(FRotator NewRotator);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetMuzzleRotation(FRotator NewRotator);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SetGun(class AWeapon_Master *NewItem);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_WeaponReload();

	void AimMissile();
	void UnAimMissile();
	void WearItem();


public:
	//Classes
	TSubclassOf<class AWeapon_Master>WeaponMasterClass;
	TSubclassOf<class UFPSHudWidget>FPSHudClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Direction;
		FVector MissileVelocity;

		class AWeapon_Master* Gun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* EquippedItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* PrimaryWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AWeapon_Master* SecondaryWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UFPSHudWidget* HudWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ECamInUse CamInUse = ECamInUse::TE_TPCam;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsItemEquipped;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EWeaponSlot WeaponSlotUse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool DoPickupLinetrace;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class AObject_Master* PickupItem;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RocketSpeed = 1000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class USplineMeshComponent*> PointsArray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool ShowPath = false;
	TSubclassOf<class AMissile>MissileClass;
	UStaticMesh* SplineStaticMesh;
	class UMaterial* SplineStaticMaterial;
	FOnSkillReleased OnSkillReleased;
	//Variable

	bool IsAiming = false;
	bool IsFiring = false;
	bool IsReloading = false;
	bool CanAim = false;
	FVector GeneralCameraPosition;
	FVector MissileCameraPosition;
	float GeneralTargetArmLength = 250;
	float MissileTargetArmLength = 500;
	float GeneralWalkSpeed = 600;
	float SteamPackWalkSpeed = 1000;
	float GeneralAcceleration = 2048.0;
	float SteamPackAcceleration = 4000;
//Component
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* ADSCam_;
	TSubclassOf<class AObject_Master> GunClass;


};
