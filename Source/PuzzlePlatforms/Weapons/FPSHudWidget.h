// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UFPSHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateWeapon(class AWeapon_Master* NewWeapon);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AWeapon_Master * Weapon;

};
