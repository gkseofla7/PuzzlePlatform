// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"../PuzzlePlatforms.h"
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
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		class UCastBarWidget* DisplayCastBar(class AAbility* CastToAbility);

	void ToggleSpellBook();
	void BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat);
	void BindCharacterName(FText NewName);
	void SetLevel();
	void ToggleMap();

private:


public:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MpBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ExpBar;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Level;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HpNum;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MpNum;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UMapWidget* MapWidget;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UMinimapWidget* Minimap_Widget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UCastBarWidget* CastBar_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarWidget* ActionBar_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UBuffPanel_UI* BuffPanel_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class USpellbook_UI* Spellbook_UI;


	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat;
};
