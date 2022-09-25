// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "PuzzlePlatforms.h"
#include "GameFramework/GameModeBase.h"
#include "PuzzlePlatformsGameMode.generated.h"

UCLASS(minimalapi)
class APuzzlePlatformsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APuzzlePlatformsGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	void RespawnRequested(APlayerController* PlayerController, FTransform SpawnTransform, int TeamNum);
	void PossessPlayer(APlayerController* PlayerController, class ACharacter_Master* NewPlayer);




public:
	TSubclassOf<class ACharacter_Master> BPSoldierClass;
	TSubclassOf<class ACharacter_Master> BPWarriorClass;
};



