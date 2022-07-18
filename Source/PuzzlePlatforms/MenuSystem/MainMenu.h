// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPalyers;
	uint16 MaxPalyers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()



protected:
	virtual bool Initialize();

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuHostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuCancelButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostName;
		
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	//UPROPERTY(meta = (BindWidget))
	//class UEditableTextBox* IPAddressField;
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenHostMenu();
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void CloseHostMenu();

	UFUNCTION()
	void CloseJoinMenu();



	UFUNCTION()
	void QuitPressed();

	void LoadServerRow();

	TOptional<uint32> SelectedIndex;

private:
	TSubclassOf<class UUserWidget>ServerRowClass;

	void UpdateChildren();

};
