// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PuzzlePlatforms.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"
#include "MyPlayerData.h"


#include "PuzzlePlatformsGameInstance.generated.h"



/**
 * 
 */


UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void LoadMainMenu() override;
	virtual void RefreshServerList() override;

	UFUNCTION(exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(exec, BlueprintCallable)
		void LoadCloseMenu();

	UFUNCTION(BlueprintCallable)
		void LoadSetNameMenu(APlayerController* NewPlayer);

	UFUNCTION(exec)
		void Host(FText HostName) override;
	UFUNCTION(exec)
		void  Join(uint32 Index) override;
	void StartSession();

	FMyCharacterrData* GetMyCharacterData(int32 Level);
	




private:
	TSubclassOf<class UUserWidget>MenuClass;
	TSubclassOf<class UUserWidget>CloseMenuClass;
	TSubclassOf<class UUserWidget> SetNameMenuClass;

	class UMainMenu* Menu;
	class UQuitMenu* QuitMenu;
	class USetNameMenu* SetNameMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TOptional<FText>HostName;
	UPROPERTY()
	UDataTable* MyCharacterTable;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();
};
