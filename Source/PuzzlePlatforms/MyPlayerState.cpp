// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Character_Master.h"
#include "PlayersComponent/MyCharacterStatComponent.h"
#include "DataTable/MyPlayerData.h"
#include "MyPlayerController.h"

AMyPlayerState::AMyPlayerState()
{
	GameScore = 0;
	SpellsUpgrade.SetNum(10);
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}

void AMyPlayerState::BeginPlay()//어차피 각각 자기꺼 시킴
{
	Super::BeginPlay();
	//여기서 그냥 초기화 시키면 됨
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server PlayerState Beginplay"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client PlayerState Beginplay"));
	}
	PlayerLevel = 1;
	CharacterStat->SetLevel(PlayerLevel);
	auto MyPawn = GetPawn();
	if (MyPawn == nullptr) return;
	auto MyPlayer = Cast<ACharacter_Master>(MyPawn);
	if (MyPlayer == nullptr) return;


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
	CharacterStat->SetLevel(PlayerLevel);
}

bool AMyPlayerState::NetMulticast_InitializeCharacterStat_Validate ()
{
	return true;
}

void AMyPlayerState::Server_SpellsUpgrade_Implementation(int index)
{
	NetMulticast_SpellsUpgrade(index);
}

bool AMyPlayerState::Server_SpellsUpgrade_Validate(int index)
{
	return true;
}
void AMyPlayerState::NetMulticast_SpellsUpgrade_Implementation(int index)
{
	SpellsUpgrade[index]++;
}

bool AMyPlayerState::NetMulticast_SpellsUpgrade_Validate(int index)
{
	return true;
}

void AMyPlayerState::Server_SetSkillPoints_Implementation(int NewSkillPoint)
{
	NetMulticast_SetSkillPoints(NewSkillPoint);
}

bool AMyPlayerState::Server_SetSkillPoints_Validate(int NewSkillPoint)
{
	return true;
}

void AMyPlayerState::NetMulticast_SetSkillPoints_Implementation(int NewSkillPoint)
{
	SkillPoints = NewSkillPoint;
}

bool AMyPlayerState::NetMulticast_SetSkillPoints_Validate(int NewSkillPoint)
{
	return true;
}


void AMyPlayerState::Server_SetExp_Implementation(int NewExp)
{
	NetMulticast_SetExp(NewExp);
}

bool AMyPlayerState::Server_SetExp_Validate(int NewExp)
{
	return true;
}

void AMyPlayerState::NetMulticast_SetExp_Implementation(int NewExp)
{
	//음,, 더좋은 방법 없으려나;;
	ABCHECK(CharacterStat != nullptr);
	ABCHECK(CharacterStat->CurrentStatData != nullptr);
	if (NewExp >= CharacterStat->CurrentStatData->NextExp)
	{
		NewExp = NewExp - CharacterStat->CurrentStatData->NextExp;
		PlayerLevel++;
		CharacterStat->SetLevel(PlayerLevel);
	}
	Exp = NewExp;
}

bool AMyPlayerState::NetMulticast_SetExp_Validate(int NewExp)
{
	return true;
}