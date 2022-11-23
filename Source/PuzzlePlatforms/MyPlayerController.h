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
	void SetInputModeGameUI();
	void SetWidget(class UMyCharacterStatComponent* NewCharacterStat);
	void BindWidget(class UMyCharacterStatComponent* NewCharacterStat);
	//class UPlayerInfoWidget* GetHudWidget() const;
	//void PossessCharacter();
	void CheckTimeSync(float DeltaTime);
	float GetServerTime();
	UFUNCTION(Server, Reliable)
		void ServerRequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable)
		void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	//	TSubclassOf<class UUserWidget> NewHUDWidgetClass;
public:


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	//class UMyCharacterStatComponent* CharacterStat;//이게 근데 생각해보면.. controller에 있으면..다른애들 stat은 어찌함?
	UPROPERTY(BlueprintReadWrite)
		class UPlayerInfoWidget* PlayerInfoHUDWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UPlayerInfoWidget> PlayerInfoHUDWidgetClass;
	float ClientServerDelta;
	bool HasWidget = false;
	float SingleTripTime = 0.f;
	float TimeSyncRunningTime = 0.f;
	float TimeSyncFrequency = 5.f;

	//UPROPERTY()
	//	class UUserWidget* NewHUDWidget;
};
