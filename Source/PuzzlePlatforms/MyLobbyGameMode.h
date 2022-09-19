// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "MyLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMyLobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
public:
	AMyLobbyGameMode();

	virtual void Logout(AController* Exiting) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	uint32 NumberOfReady = 0;
	uint32 NumberOfPlayers = 0;
	void StartGame();
private:


	FTimerHandle GameStartTimer;

};
