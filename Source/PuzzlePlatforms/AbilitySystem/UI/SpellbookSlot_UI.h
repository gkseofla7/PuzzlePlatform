// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellbookSlot_UI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API USpellbookSlot_UI : public UUserWidget
{
	GENERATED_BODY()
public:
	//void 	NativePreConstruct() override;
	bool Initialize();
	UFUNCTION(BlueprintCallable)
	void CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass);
	UFUNCTION()
		void UpgradeSkill();
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UImage* SpellImage;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UButton* UpgradeButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UTextBlock* T_UpgradeNum;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UProgressBar* DisableBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AAbility> AbilityClass;

	int32 SlotNum;
};
