// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsCharacter.h"
#include "Soldier.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ASoldier : public APuzzlePlatformsCharacter
{
	GENERATED_BODY()
public:
	ASoldier();

protected:
	virtual void PostInitializeComponents() override;

private:

};
