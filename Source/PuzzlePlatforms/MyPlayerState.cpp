// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "PuzzlePlatformsCharacter.h"
#include "PlayersComponent/MyCharacterStatComponent.h"

AMyPlayerState::AMyPlayerState()
{
//	CharacterLevel = 1;
	GameScore = 0;
}


int32 AMyPlayerState::GetGameScore() const
{
	return GameScore;
}


void AMyPlayerState::InitPlayerData()
{
	//SetPlayerName(TEXT("Destiny"));
	//CharacterLevel = 5;
	GameScore = 0;
}

void AMyPlayerState::Server_SetPlayerName_Implementation(const FText& NewPlayerName)
{
	SetPlayerName(NewPlayerName.ToString());
}

bool AMyPlayerState::Server_SetPlayerName_Validate(const FText& NewPlayerName)
{
	return true;
}


void AMyPlayerState::Server_SetPlayerLevel_Implementation(const int32& NewLevel)
{
	NetMulticast_SetPlayerLevel(NewLevel);
}
bool AMyPlayerState::Server_SetPlayerLevel_Validate(const int32& NewLevel)
{
	return true;
}


void AMyPlayerState::NetMulticast_SetPlayerLevel_Implementation(const int32& NewLevel)
{
	PlayerLevel = NewLevel;
}

bool AMyPlayerState::NetMulticast_SetPlayerLevel_Validate(const int32& NewLevel)
{
	return true;
}


void AMyPlayerState::Server_InitializeCharacterStat_Implementation()//입장했을때 내꺼를 다른 애들에게 다 초기화시킴
{
	NetMulticast_InitializeCharacterStat();
}

bool AMyPlayerState::Server_InitializeCharacterStat_Validate()
{
	return true;
}

void AMyPlayerState::NetMulticast_InitializeCharacterStat_Implementation()
{
	auto MyPawn = GetPawn();
	if (MyPawn != nullptr)
	{
		auto MyCharacter = Cast<APuzzlePlatformsCharacter>(MyPawn);
		if (MyCharacter != nullptr)
		{
			MyCharacter->CharacterStat->SetLevel(PlayerLevel);
		}
	}
}

bool AMyPlayerState::NetMulticast_InitializeCharacterStat_Validate ()
{
	return true;
}