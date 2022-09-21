// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMapWidget(const FObjectInitializer& ObjectInitializer);
	void AddsPOI(AActor* Owner);


public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UOverlay* BigMapOverlay;
	TSubclassOf<class UPointOfInterestWidget> PointOfInterestWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Dimensions = 35000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Zoom = 0.5;
};
