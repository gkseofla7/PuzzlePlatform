// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Character_Master.h"
#include "PlayersComponent/MyCharacterStatComponent.h"
#include "DataTable/MyPlayerData.h"
#include "MyPlayerController.h"
#include "MyLobbyGameMode.h"

AMyPlayerState::AMyPlayerState()
{
	GameScore = 0;
	SpellsUpgrade.SetNum(10);
	for (int i = 0; i < 10; i++)
	{
		SpellsUpgrade[i] = 0;
	}

	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}

void AMyPlayerState::BeginPlay()//������ ���� �ڱⲨ ��Ŵ
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server : PlayerState BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client : PlayerState BeginPlay"));
	}
	if(Cast<AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()))!=nullptr)
		return;
	CharacterStat->SetLevel(1);

}

int32 AMyPlayerState::GetGameScore() const
{
	return GameScore;
}



void AMyPlayerState::Server_SetPlayerName_Implementation(const FText& NewPlayerName)
{
	SetPlayerName(NewPlayerName.ToString());
}

bool AMyPlayerState::Server_SetPlayerName_Validate(const FText& NewPlayerName)
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
	SkillPoints = SkillPoints-1;
	//���� �÷��̾��� State�� ������
	if (this == (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerState<AMyPlayerState>()))
	{
		OnSkillPointChangedDelegate.Broadcast();
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server :SpellsUpgrade %d"), SpellsUpgrade[index]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client :SpellsUpgrade %d"), SpellsUpgrade[index]);
	}
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
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server : LevelUpSetSkillPoint %d"), NewSkillPoint);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client : LevelUpSetSkillPoint %d"), NewSkillPoint);
	}

	SkillPoints = NewSkillPoint;
	if (this == (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerState<AMyPlayerState>()))
	{
		OnSkillPointChangedDelegate.Broadcast();
	}

}

bool AMyPlayerState::NetMulticast_SetSkillPoints_Validate(int NewSkillPoint)
{
	return true;
}


//void AMyPlayerState::Server_SetPlayerLevel_Implementation(const int32& NewLevel)
//{
//	NetMulticast_SetPlayerLevel(NewLevel);
//}
//bool AMyPlayerState::Server_SetPlayerLevel_Validate(const int32& NewLevel)
//{
//	return true;
//}
//
//
//void AMyPlayerState::NetMulticast_SetPlayerLevel_Implementation(const int32& NewLevel)
//{
//	PlayerLevel = NewLevel;
//}
//
//bool AMyPlayerState::NetMulticast_SetPlayerLevel_Validate(const int32& NewLevel)
//{
//	return true;
//}


//void AMyPlayerState::Server_InitializeCharacterStat_Implementation()//���������� ������ �ٸ� �ֵ鿡�� �� �ʱ�ȭ��Ŵ
//{
//	NetMulticast_InitializeCharacterStat();
//}
//
//bool AMyPlayerState::Server_InitializeCharacterStat_Validate()
//{
//	return true;
//}
//
//void AMyPlayerState::NetMulticast_InitializeCharacterStat_Implementation()
//{
//	//CharacterStat->SetLevel(PlayerLevel);
//}
//
//bool AMyPlayerState::NetMulticast_InitializeCharacterStat_Validate ()
//{
//	return true;
//}