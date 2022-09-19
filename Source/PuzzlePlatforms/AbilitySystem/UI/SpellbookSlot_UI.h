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
	UFUNCTION(BlueprintCallable)
	void CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass);
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UImage* SpellImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AAbility> AbilityClass;
};
