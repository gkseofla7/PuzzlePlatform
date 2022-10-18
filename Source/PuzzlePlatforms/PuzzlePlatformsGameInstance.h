// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PuzzlePlatforms.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"
#include "DataTable/MyPlayerData.h"
#include "DataTable/MonstersData.h"
#include "AI/EnumMonsterType.h"

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
		void LoadSetNameMenu();

	UFUNCTION(exec)
		void Host(FText HostName) override;
	UFUNCTION(exec)
		void  Join(uint32 Index) override;
	void StartSession();
	void DestroySession();
	class UQuitMenu* GetQuitMenu() {
		return QuitMenu
			;
	}
	FMyCharacterrData* GetMyCharacterData(int32 Level);
	FMonsterData* GetMonsterData(int32 Level, EMonsterEnum MonsterEnuml);
	//FMyCharacterrData* GetMyCharacterData(int32 Level);



private:
	UPROPERTY()
	TSubclassOf<class UUserWidget>MenuClass;
	UPROPERTY()
	TSubclassOf<class UUserWidget>CloseMenuClass;
	UPROPERTY()
	TSubclassOf<class UUserWidget> SetNameMenuClass;
	UPROPERTY()
	TSubclassOf<class UUserWidget> NewHUDWidgetClass;
	UPROPERTY()
	TSubclassOf<class UPlayerInfoWidget> PlayerInfoHUDWidgetClass;
	UPROPERTY()
	class UMainMenu* Menu;
	UPROPERTY()
	class UQuitMenu* QuitMenu;
	UPROPERTY()
	class USetNameMenu* SetNameMenu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TOptional<FText>HostName;
	UPROPERTY()
	UDataTable* MyCharacterTable;
	UPROPERTY()
	UDataTable* NearMonsterTable;
	UPROPERTY()
	UDataTable* ArcherDataTable;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();
public:
	UPROPERTY()
	class UPlayerInfoWidget* PlayerInfoWidget;
	FText PlayerName;
	bool JoinClicked= false;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int CharacterIndex = 1;// 1번이면 Soldier 2번이면 Warrior 
};
