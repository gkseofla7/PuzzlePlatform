// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"



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
	
	UFUNCTION(exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(exec, BlueprintCallable)
		void LoadCloseMenu();

	UFUNCTION(exec)
		void Host(FText HostName) override;
	UFUNCTION(exec)
		void  Join(uint32 Index) override;

	virtual void LoadMainMenu() override;

	virtual void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget>MenuClass;
	TSubclassOf<class UUserWidget>CloseMenuClass;

	class UMainMenu* Menu;
	class UQuitMenu* QuitMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TOptional<FText>HostName;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
};
