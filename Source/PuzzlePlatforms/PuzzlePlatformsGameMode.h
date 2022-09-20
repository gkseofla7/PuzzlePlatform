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
	void PossessPlayer(APlayerController* PlayerController, class APuzzlePlatformsCharacter* NewPlayer);




public:
	int CharacterNum = 1;
	TSubclassOf<class APuzzlePlatformsCharacter> BPSoldierClass;
	TSubclassOf<class APuzzlePlatformsCharacter> BPWarriorClass;
};



