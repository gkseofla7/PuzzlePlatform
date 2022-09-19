// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragUI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UDragUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass);

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* DragIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AAbility> AbilityClass;
};
