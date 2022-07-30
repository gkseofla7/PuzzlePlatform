// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLobbyGameMode.h"
#include "GameMapsSettings.h"
#include "PuzzlePlatformsGameInstance.h"

#include "TimerManager.h"
void AMyLobbyGameMode::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	

	if (NumberOfPlayers >= 2)
	{
		//FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AMyLobbyGameMode::StartPlay,);
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &AMyLobbyGameMode::StartGame, 5.0f);

	}
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
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}