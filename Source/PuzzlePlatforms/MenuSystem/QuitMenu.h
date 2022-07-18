// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "QuitMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UQuitMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;


	UFUNCTION()
	void CancelMenu();

	UFUNCTION()
	void BackToMainMenu();
};
