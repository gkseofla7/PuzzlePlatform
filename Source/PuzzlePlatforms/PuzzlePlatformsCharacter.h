// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include"PuzzlePlatforms.h"
#include "GameFramework/Character.h"
#include "PlayersComponent/MotionReplicatorInterface.h"
#include "PuzzlePlatformsCharacter.generated.h"

UCLASS(config=Game)
class APuzzlePlatformsCharacter : public ACharacter
{
	GENERATED_BODY()

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

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnResetVR();
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void GetInTheCar();
	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	void Attack();


	//UFUNCTION()
	//	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


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

public:

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UMyCharacterStatComponent* CharacterStat;
	//UPROPERTY(VisibleAnywhere, Category = Replicator)
	//	class UPlayersMotionReplicator* MotionReplicator;
	//UPROPERTY에서 돌아가려면
	UPROPERTY(VisibleAnywhere, Category = Replicator)
	TScriptInterface<IMotionReplicatorInterface> MotionReplicator;
		//IMotionReplicatorInterface* MotionReplicator;

	

protected:
	UPROPERTY()
		class UPlayerAnimInstance* MyAnim;
	


};

