// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"
#include "Soldier.h"
#include "Warrior.h"
#include "PuzzlePlatformsGameInstance.h"

void AMyDefaultPawn::BeginPlay()
{
	Super::BeginPlay();

	if ( IsLocallyControlled() == true)
	{
		auto MyGameInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
		if (MyGameInstance != nullptr)
		{
			Server_PossessCharacter(Cast<APlayerController>(GetController()), MyGameInstance->CharacterIndex);
			UnPossessed();
			return;
		}
	}
}
void AMyDefaultPawn::Server_PossessCharacter_Implementation(APlayerController* NewController, int Index)
{
	auto MyInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (MyInstance != nullptr)
	{
		if (Index == 1)
		{
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoldier::StaticClass(), AllActors);
			for (auto AllActor : AllActors)
			{
				auto Soldier = Cast<ASoldier>(AllActor);
				if (Soldier->bPossessed == false)
				{
					Soldier->bPossessed = true;
					NewController->Possess(Soldier);

					return;
				}
			}
		}

		else if (Index == 2)
		{
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWarrior::StaticClass(), AllActors);
			for (auto AllActor : AllActors)
			{
				auto Warrior = Cast<AWarrior>(AllActor);
				if (Warrior->bPossessed == false)
				{
					Warrior->bPossessed = true;
					NewController->Possess(Warrior);

					return;
				}
			}
		}
	}
}

bool AMyDefaultPawn::Server_PossessCharacter_Validate(APlayerController* NewController, int Index)
{
	return true;
}