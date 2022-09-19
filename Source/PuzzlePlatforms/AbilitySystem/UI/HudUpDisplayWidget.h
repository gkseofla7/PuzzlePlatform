// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudUpDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UHudUpDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UHudUpDisplayWidget(const FObjectInitializer& ObjectInitializer);
		
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		class UCastBarWidget* DisplayCastBar(class AAbility* CastToAbility);

public:
	void ToggleSpellBook();

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UCastBarWidget* CastBar_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UActionBarWidget* ActionBar_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UBuffPanel_UI* BuffPanel_UI;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class USpellbook_UI* Spellbook_UI;
	//TSubclassOf<class UUserWidget>CastBarClass;
};
