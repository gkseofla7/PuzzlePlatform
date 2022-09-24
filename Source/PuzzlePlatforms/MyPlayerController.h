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
	void SetWidget(class UMyCharacterStatComponent* NewCharacterStat);
	void BindWidget(class UMyCharacterStatComponent* NewCharacterStat);
	//class UPlayerInfoWidget* GetHudWidget() const;
	//void PossessCharacter();

protected:
	virtual void BeginPlay() override;


	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	//	TSubclassOf<class UUserWidget> NewHUDWidgetClass;
public:


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	//class UMyCharacterStatComponent* CharacterStat;//�̰� �ٵ� �����غ���.. controller�� ������..�ٸ��ֵ� stat�� ������?
	UPROPERTY(BlueprintReadWrite)
		class UPlayerInfoWidget* PlayerInfoHUDWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UPlayerInfoWidget> PlayerInfoHUDWidgetClass;
	bool HasWidget = false;

	//UPROPERTY()
	//	class UUserWidget* NewHUDWidget;
};
