// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UActionBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarSlotWidget* ActionBarSlot_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarSlotWidget* ActionBarSlot_UI_1;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarSlotWidget* ActionBarSlot_UI_2;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarSlotWidget *ActionBarSlot_UI_3;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarSlotWidget* ActionBarSlot_UI_4;
};
