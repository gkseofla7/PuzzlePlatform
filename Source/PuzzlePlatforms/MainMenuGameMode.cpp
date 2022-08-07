// Fill out your copyright notice in the Description page of Project Settings.
#include "MainMenuGameMode.h"
#include "NoPawn.h"


AMainMenuGameMode::AMainMenuGameMode()
{

	DefaultPawnClass = ANoPawn::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
}