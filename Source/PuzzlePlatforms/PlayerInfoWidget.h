// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"PuzzlePlatforms.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();
private:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MpBar;

public:
	void BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat);

private:
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;
};
