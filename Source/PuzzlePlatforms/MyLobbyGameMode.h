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


	virtual void Logout(AController* Exiting) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;


private:
	uint32 NumberOfPlayers = 0;
	FTimerHandle GameStartTimer;
	void StartGame();
};
