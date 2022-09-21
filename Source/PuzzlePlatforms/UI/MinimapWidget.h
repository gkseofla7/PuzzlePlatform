// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
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
	UMinimapWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddPOI(AActor * Owner);
	void AddsPOI(AActor* Owner);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dimensions= 35000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Zoom = 0.5;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UOverlay* MapOverlay;
	TSubclassOf<class UPointOfInterestWidget> PointOfInterestWidgetClass;
};
