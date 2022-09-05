// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsGameMode.h"
#include "PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

#include "MyPlayerState.h"
#include "PuzzlePlatformsGameInstance.h"
#include "MyPlayerController.h"
#include "Cars/MyProjectPawn.h"

APuzzlePlatformsGameMode::APuzzlePlatformsGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Soldier_2"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Soldier"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Warrior"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//DefaultPawnClass = AMyProjectPawn::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

	
}

//void APuzzlePlatformsGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//}

void APuzzlePlatformsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto MyPlaerState = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	//ABCHECK(nullptr != MyPlaerState)
	MyPlaerState->InitPlayerData();
}