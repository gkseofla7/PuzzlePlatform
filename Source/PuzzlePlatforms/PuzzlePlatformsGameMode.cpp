// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsGameMode.h"
#include "Character_Master.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "MyDefaultPawn.h"

#include "MyPlayerState.h"
#include "PuzzlePlatformsGameInstance.h"
#include "MyPlayerController.h"
#include "Cars/MyProjectPawn.h"
#include "GameFramework/DefaultPawn.h"

APuzzlePlatformsGameMode::APuzzlePlatformsGameMode()
{
	DefaultPawnClass = AMyDefaultPawn::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Karts/BP_GoKart"));
	static ConstructorHelpers::FClassFinder<APawn> SoldierBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Soldier_2"));
	static ConstructorHelpers::FClassFinder<APawn> WarriorBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_Warrior"));

	if (SoldierBPClass.Class != NULL)
	{

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



}


void APuzzlePlatformsGameMode::RespawnRequested(APlayerController* PlayerController, FTransform SpawnTransform, int TeamNum)//서버에서만함
{
	ABCHECK(PlayerController != nullptr);
	if (HasAuthority())
	{
		ACharacter_Master* Player = nullptr;
		if(TeamNum==1)
			Player = GetWorld()->SpawnActor<ACharacter_Master>(BPSoldierClass, SpawnTransform);
		else if(TeamNum ==2)
			Player = GetWorld()->SpawnActor<ACharacter_Master>(BPWarriorClass, SpawnTransform);
		
		//Delay 해줌 0.2초
		if (Player == nullptr)
			return;
		FTimerHandle UniqueHandle;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &APuzzlePlatformsGameMode::PossessPlayer, PlayerController, Player);
		GetWorldTimerManager().SetTimer(UniqueHandle, RespawnDelegate, .5f, false);

	}

}

void APuzzlePlatformsGameMode::PossessPlayer(APlayerController* PlayerController, class ACharacter_Master* NewPlayer)
{
	PlayerController->Possess(NewPlayer);
	auto Controller = Cast< AMyPlayerController>(PlayerController);

}

