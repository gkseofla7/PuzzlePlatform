// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Buff_Master.h"
#include "Ability_Buff.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEndBuffDelegate);

UCLASS()
class PUZZLEPLATFORMS_API AAbility_Buff : public AAbility_Buff_Master
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();

};
