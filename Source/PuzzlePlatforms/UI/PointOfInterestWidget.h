// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Blueprint/UserWidget.h"


#include "PointOfInterestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPointOfInterestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPointOfInterestWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
	void CustomInitialize(AActor* NewOwner, bool NewIsStatic);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	float FindAngle(FVector2D A, FVector2D B);
	FVector2D FindCoord(float Radius, float Angle);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStatic;
	TSubclassOf<UActorComponent> PointOfInterestComponentClass;
	UPROPERTY(meta = (BindWidget),BlueprintReadWrite)
		class UImage* CustomImage;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UThrobber* DefaultImage;

};
