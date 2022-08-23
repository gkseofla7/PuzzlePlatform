// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PuzzlePlatforms.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	void SetInputModeGameAndUI();
	void SetInputModeGame();
	class UPlayerInfoWidget* GetHudWidget() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UPlayerInfoWidget> HUDWidgetClass;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	//	TSubclassOf<class UUserWidget> NewHUDWidgetClass;
public:
	UPROPERTY()
	class UPlayerInfoWidget* HUDWidget;
	//UPROPERTY()
	//	class UUserWidget* NewHUDWidget;
};
