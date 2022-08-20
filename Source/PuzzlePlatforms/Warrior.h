// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsCharacter.h"
#include "Warrior.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AWarrior : public APuzzlePlatformsCharacter
{
	GENERATED_BODY()

	AWarrior();
protected:
	virtual void PostInitializeComponents() override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;;

UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void JumpFromWall();
void ClimbTheWall();
private:
	void AttackCheck();

private:	
	//UPROPERTY()
	//class UPlayerAnimInstance* MyAnim;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class ASword_Master* EquippedItem;
	TSubclassOf<class ASword_Master>SwordClass;
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
};
