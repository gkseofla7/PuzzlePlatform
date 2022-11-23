// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Master.h"
#include "Warrior.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AWarrior : public ACharacter_Master
{
	GENERATED_BODY()

	AWarrior();
protected:
	virtual void PostInitializeComponents() override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void JumpFromWall();
	UFUNCTION()
	void EndAnimation(UAnimMontage* Montage, bool bInterrupted);
	bool ForwardTrace();
	void HeightTrace();
	void Hang();

	UFUNCTION(BlueprintCallable)
	void ClimbUp();
	void HangMontageNotify();
	UFUNCTION(BlueprintCallable)
		void DropDown();
	//void ClimbTheWall();

	void Climb();

private:
	void AttackCheck();

public:
	void Dash();
	void PlayHangToCrouchMontage();
	void Die() override;
	void PlayersDied();
	void RespawnCharacter();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RespawnPawn(APlayerController* NewController);
	void SetbIsAttacking(bool NewbIsAttacking);
	virtual void Attack();
private:	
	//UPROPERTY()
	//class UPlayerAnimInstance* MyAnim;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsClimbing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveUpDown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveRightLeft;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool ClimbTheWallOn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsOnEdge;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsClimbingUp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector WallNormal;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector WallLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector HeightLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UUserWidget* HudWidget;
	bool bNextAttack = false;
	bool bNextAttackStart = false;


};
