// Fill out your copyright notice in the Description page of Project Settings.

#include "NoPawn.h"
#include "MainMenuGameMode.h"

AMainMenuGameMode::AMainMenuGameMode()
{

	DefaultPawnClass = ANoPawn::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
}