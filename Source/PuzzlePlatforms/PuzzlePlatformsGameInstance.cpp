// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"


#include "PlatformTrigger.h"
#include "MenuSystem/QuitMenu.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/SetNameMenu.h"

const static FName SESSION_NAME = NAME_GameSession;

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> CloseMenuBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_InGameMenu"));
	if (!ensure(CloseMenuBPClass.Class != nullptr)) return;
	CloseMenuClass = CloseMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> SetNameMenuBPClass(TEXT("/Game/PuzzlePlatforms/MenuSystem/WBP_SetName"));
	if (!ensure(SetNameMenuBPClass.Class != nullptr)) return;
	SetNameMenuClass = SetNameMenuBPClass.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(TEXT("/Game/PlayerData/MyPlayerData"));
	if (DT_MYCHARACTER.Succeeded()) {
		MyCharacterTable = DT_MYCHARACTER.Object;
		ABCHECK(MyCharacterTable->GetRowMap().Num() > 0);
	}

	//static ConstructorHelpers::FClassFinder< UUserWidget> NEWUI_HUD_C(TEXT("/Game/AbilitySystem/UI/HeadsUpDisplay"));
	static ConstructorHelpers::FClassFinder< UUserWidget> NEWUI_HUD_C(TEXT("/Game/AbilitySystem/HeadsUpDisplay"));
	if (NEWUI_HUD_C.Succeeded())
	{
		NewHUDWidgetClass = NEWUI_HUD_C.Class;
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	//이게 무엇보다 일찍 시작함
	// UGameInstance::Init -> AActor::PostInitializeComponents -> AGameMode::PostLogin -> AGameMode::StartPlay
	// ->AActor::BeginPlay
	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
	auto Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Subsystem"));
	}
	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);


	}
	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}

	HeadsUpDisplay = CreateWidget<UUserWidget>(this, NewHUDWidgetClass);


}
FMyCharacterrData* UPuzzlePlatformsGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterrData>(*FString::FromInt(Level), TEXT(""));
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	Menu->Setup();

	Menu->SetMenuInterface(this);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	auto Pawn = PlayerController->GetPawn();
	Pawn->DisableInput(PlayerController);
	
}

void UPuzzlePlatformsGameInstance::LoadCloseMenu()
{
	if (!ensure(CloseMenuClass != nullptr)) return;

	QuitMenu = CreateWidget<UQuitMenu>(this, CloseMenuClass);

	QuitMenu->Setup();

	QuitMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadSetNameMenu()
{
	if (!ensure(SetNameMenuClass != nullptr)) return;

	SetNameMenu = CreateWidget<USetNameMenu>(this, SetNameMenuClass);

	SetNameMenu->Setup();

	//SetNameMenu->PlayerController = NewPlayer;


}
void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
	
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(TEXT("TEST"), HostName->ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}
void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not able to create session"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Why Not?"));
	if (Menu != nullptr)
		Menu->Teardown();


	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
	//World->ServerTravel("/Game/PuzzlePlatforms/Maps/L_Menu'?listen");
}
void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success&&SessionSearch.IsValid()&&Menu!= nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Session!"));
		TArray<FServerData> ServerNames;


		for (auto& Result : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session %s"), *Result.GetSessionIdStr());
			FServerData Data;
			Data.Name = Result.GetSessionIdStr();
			Data.HostUsername = Result.Session.OwningUserName;
			Data.MaxPalyers = Result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPalyers = Data.MaxPalyers - Result.Session.NumOpenPublicConnections;
			FString TestSettings;
			if (Result.Session.SessionSettings.Get(TEXT("Test"), TestSettings))
			{
				UE_LOG(LogTemp, Warning, TEXT("Data found in settings: %s"), *TestSettings);
				Data.Name = TestSettings;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Didn't get expected data"));
			}

			ServerNames.Add(Data);
		}

			Menu->SetServerList(ServerNames);
	}

}

void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPuzzlePlatformsGameInstance::Host(FText _HostName)
{
	HostName = _HostName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}

	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{

	//FString str = FString::Printf(TEXT("Join IP : %s"), *IPAddress);
	////GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, *str);

	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Joining %s"), *IPAddress));

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		
		Menu->Teardown();
		//RefreshServerList();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);


}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Type::Success)
		return;
	FString ConnectInfo;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not Get connect string."));
		return;
	}


	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	
	PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
}
void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/PuzzlePlatforms/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::RefreshServerList()
{

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Let's find Session~"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	}

}