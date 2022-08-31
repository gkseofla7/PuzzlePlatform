// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include"PuzzlePlatforms.h"
#include "GameFramework/Character.h"
//#include "PlayersComponent/MotionReplicatorInterface.h"
#include "PlayersComponent/SoldierMotionReplicator.h"
#include "PlayersComponent/MotionInterfaceComponent.h"
#include "PuzzlePlatformsCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptCastingDeleagate);

UCLASS(config=Game, BlueprintType)
class APuzzlePlatformsCharacter : public ACharacter
{
protected:
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NearObjectCollisionDetector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	APuzzlePlatformsCharacter();
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	FRotator GetMuzzleRotation();
	void Skill1Clicked();
	void Skill2Clicked();
	void Skill3Clicked();
	void Skill4Clicked();
	void Skill5Clicked();
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void GetInTheCar();
	void SeeMouseCursur();
	virtual void Attack();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	void SetTargetPlayerWithLineTrace();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill1Clicked();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill2Clicked();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill3Clicked();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill4Clicked();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill5Clicked();


public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Stat)
		class UMyCharacterStatComponent* CharacterStat;
	//UPROPERTY(VisibleAnywhere, Category = Replicator)
	//	class UPlayersMotionReplicator* MotionReplicator;
	//UPROPERTY에서 돌아가려면
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
	//TScriptInterface<IMotionReplicatorInterface> DaerimMotionReplicator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		UMotionInterfaceComponent* DaerimMotionReplicator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		class UDecalComponent* DecalComponent;
		//IMotionReplicatorInterface* MotionReplicator;
	bool MouseCursorToggle = false;
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FInterruptCastingDeleagate InterruptCasting;
	class UHudUpDisplayWidget* HeadsUpDisplayRef;
	bool SkillAvailable = true;
	APuzzlePlatformsCharacter* TargetPlayer;

protected:
	UPROPERTY()
		UAnimInstance* MyAnim;
	


};

