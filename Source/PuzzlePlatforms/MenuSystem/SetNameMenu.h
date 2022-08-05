// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "SetNameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API USetNameMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerID;



	UFUNCTION()
		void ConfirmID();

public:
	APlayerController* PlayerController;
};
