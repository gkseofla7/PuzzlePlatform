// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLobbyGameMode.h"
#include "GameMapsSettings.h"
#include "PuzzlePlatformsGameInstance.h"
#include "GameFramework/PlayerState.h"

#include "TimerManager.h"

AMyLobbyGameMode::AMyLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> LobbyBPCharacter(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_LobbyCharacter"));
	if (LobbyBPCharacter.Class != NULL)
	{
		DefaultPawnClass = LobbyBPCharacter.Class;
	}
	PlayerStateClass = APlayerState::StaticClass();
}
void AMyLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void AMyLobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)return;
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	bUseSeamlessTravel = true;

	GameInstance->StartSession();
	//World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/L_MainLevel?listen");
}

void AMyLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;


	//Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->LoadSetNameMenu(NewPlayer);
	if (NumberOfPlayers == NumberOfReady && NumberOfPlayers % 2 == 0)
	{
		//FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AMyLobbyGameMode::StartPlay,);
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &AMyLobbyGameMode::StartGame, 10.0f);

	}
}