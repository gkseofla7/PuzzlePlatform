// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::CloseJoinMenu);

	if (!ensure(JoinJoinMenuButton != nullptr)) return false;
	JoinJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(HostMenuHostButton != nullptr)) return false;
	HostMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(HostMenuCancelButton != nullptr)) return false;
	HostMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::CloseHostMenu);
	
	return true;
}



void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
	if (MenuInterface != nullptr)
	{
		FText SendHostName = HostName->GetText();
		MenuInterface->Host(SendHostName);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	ServerList->ClearChildren();
	uint32 i = 0;

	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(GetWorld(), ServerRowClass);
		if (!ensure(Row != nullptr)) return;


		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionText = FString::Printf(TEXT(" % d / % d"), ServerData.CurrentPalyers, ServerData.MaxPalyers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}


}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();

}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{

		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex == i);
		}
	
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet()&& MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());

		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}

}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);

}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;	MenuSwitcher->SetActiveWidget(JoinMenu);


	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}

}
void UMainMenu::CloseHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);


}
void UMainMenu::CloseJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);


}

void UMainMenu::QuitPressed()
{

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::LoadServerRow()
{


	//Menu->Setup();

	//Menu->SetMenuInterface(this);
}