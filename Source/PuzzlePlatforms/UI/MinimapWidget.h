// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddPOI(AActor * Owner);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dimensions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Zoom;
	
};
