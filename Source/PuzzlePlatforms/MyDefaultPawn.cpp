// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"
#include "Soldier.h"
#include "Warrior.h"
#include "Sections/RespawnSection.h"
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

		}
	}
}
void AMyDefaultPawn::Server_PossessCharacter_Implementation(APlayerController* NewController, int Index)
{
	auto MyInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (MyInstance != nullptr)
	{
		TArray<AActor*>Respawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnSection::StaticClass(), Respawns);
		FTransform RespawnTransform;
		if (Respawns.Num() == 0)
			return;
		if (Index == 1)
		{
			for (auto RespawnActor : Respawns)
			{
				auto Respawn = Cast< ARespawnSection>(RespawnActor);
				if (Respawn->TeamNum == Index)//리스폰 지역이 맞다면
				{
					RespawnTransform = Cast< ARespawnSection>(Respawn)->GetRandomTransform();
					break;
				}
			}
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoldier::StaticClass(), AllActors);
			for (auto AllActor : AllActors)
			{
				auto Soldier = Cast<ASoldier>(AllActor);
				if (Soldier->bPossessed == false)
				{


					Soldier->bPossessed = true;
					NewController->Possess(Soldier);
					Soldier->SetActorTransform(RespawnTransform);
					return;
				}
			}
		}
		else if (Index == 2)
		{
			for (auto RespawnActor : Respawns)
			{
				auto Respawn = Cast< ARespawnSection>(RespawnActor);
				if (Respawn->TeamNum == Index-1)//리스폰 지역이 맞다면
				{
					RespawnTransform = Cast< ARespawnSection>(Respawn)->GetRandomTransform();
					break;
				}
			}
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWarrior::StaticClass(), AllActors);
			for (auto AllActor : AllActors)
			{
				auto Warrior = Cast<AWarrior>(AllActor);


				if (Warrior->bPossessed == false)
				{
					Warrior->bPossessed = true;
					NewController->Possess(Warrior);
					Warrior->SetActorTransform(RespawnTransform);
					return;
				}
			}
		}
	}

	Destroy();
}

bool AMyDefaultPawn::Server_PossessCharacter_Validate(APlayerController* NewController, int Index)
{
	return true;
}