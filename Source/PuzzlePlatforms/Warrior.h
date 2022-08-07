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

	//UPROPERTY(VisibleAnywhere, Category = Replicator)
	//	class UPlayersMotionReplicator* MotionReplicator;
private:
	void AttackCheck();

private:	
	//UPROPERTY()
	//class UPlayerAnimInstance* MyAnim;
	
};
