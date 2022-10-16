// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UFPSTargetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayShootingAnim();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayUnShootingAnim();


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float crosshair_spread = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float crosshair_thickness = 5.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float crosshair_length = 25.f;


};
