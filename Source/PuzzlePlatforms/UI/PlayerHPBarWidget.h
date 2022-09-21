// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPlayerHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat);
	void UpdateHPBar();
	void UpdateMPBar();
	void UpdateLevelText();
	void UpdateNameText();
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Level;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Name;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_HPBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_MPBar;

	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;
};


