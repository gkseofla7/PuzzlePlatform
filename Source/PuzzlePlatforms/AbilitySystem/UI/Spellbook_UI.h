// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSpellbookInterface.h"
#include "PlayerStateSpellbookInterface.h"
#include "Spellbook_UI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API USpellbook_UI : public UUserWidget
{
	GENERATED_BODY()
public:
	USpellbook_UI(const FObjectInitializer& ObjectInitializer);
	void 	NativeConstruct() override;
	void CustomInitialize();
	void SetSkillPoints(int NewSkillPoint);
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UScrollBox* SpellbookContainer;
	TSubclassOf<class USpellbookSlot_UI>SpellbookSlotClass;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UWrapBox* Container;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UTextBlock* T_SkillPoints;
	ICharacterSpellbookInterface* CharacterInterface;
	IPlayerStateSpellbookInterface* PlayerStateInterface;
};
