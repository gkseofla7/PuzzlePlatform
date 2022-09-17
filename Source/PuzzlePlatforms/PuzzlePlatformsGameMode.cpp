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
	static ConstructorHelpers::FClassFinder<APawn> SoldierBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Soldier_2"));
	static ConstructorHelpers::FClassFinder<APawn> WarriorBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Warrior"));
	if (SoldierBPClass.Class != NULL)
	{
		DefaultPawnClass = SoldierBPClass.Class;
		BPSoldierClass = SoldierBPClass.Class;
	}
	if (WarriorBPClass.Class != NULL)
	{
		BPWarriorClass = WarriorBPClass.Class;
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
	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	auto MyPlaerState = Cast<AMyPlayerState>(NewPlayer->PlayerState);

	//ABCHECK(nullptr != MyPlaerState)
	MyPlaerState->InitPlayerData();
}


void APuzzlePlatformsGameMode::RespawnRequested(APlayerController* PlayerController, FTransform SpawnTransform)//서버에서만함
{
	ABCHECK(PlayerController != nullptr);
	if (HasAuthority())
	{
		auto Player = GetWorld()->SpawnActor<APuzzlePlatformsCharacter>(BPSoldierClass, SpawnTransform);
		//Delay 해줌 0.2초

		FTimerHandle UniqueHandle;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &APuzzlePlatformsGameMode::PossessPlayer, PlayerController, Player);
		GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, .5f, false);

	}
	else
	{

	}
}

void APuzzlePlatformsGameMode::PossessPlayer(APlayerController* PlayerController, class APuzzlePlatformsCharacter* NewPlayer)
{
	PlayerController->Possess(NewPlayer);
	auto Controller = Cast< AMyPlayerController>(PlayerController);

}

