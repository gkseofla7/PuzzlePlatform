// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffIcon_UI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBuffIcon_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CustomInitialize(class AAbility_Buff_Master* BuffAbility);
	void UpdateProgressBar();
	UFUNCTION()
	void DeleteFromParent();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAbility_Buff_Master* BuffAbility;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UImage* BuffImage;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UProgressBar* BuffTimer;

	float BuffLifeSpan;
};
