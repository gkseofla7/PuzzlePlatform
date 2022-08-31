// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffPanel_UI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UBuffPanel_UI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddBufftoUI(class AAbility_Buff_Master* Buff);

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UWrapBox* BuffPanel;
};
