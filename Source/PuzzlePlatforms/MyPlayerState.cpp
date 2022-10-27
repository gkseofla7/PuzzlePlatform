// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Character_Master.h"
#include "PlayersComponent/MyCharacterStatComponent.h"
#include "DataTable/MyPlayerData.h"
#include "MyPlayerController.h"
#include "MyLobbyGameMode.h"
#include "AbilitySystem/ActorAbilities.h"
#include "AbilitySystem/Ability.h"

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

void AMyPlayerState::BeginPlay()//어차피 각각 자기꺼 시킴
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

	//여기서 애들 Stat 초기화 시킴


}

//void AMyPlayerState::SetMaxLevel()
//{
//	if (SpellsMaxUpgrade.Num() != 0)
//		return;
//	auto MyCharacter = Cast<ACharacter_Master>(GetPawn());
//	if(MyCharacter!=nullptr)
//	{
//		TArray<TSubclassOf<AAbility>> Spells = MyCharacter->ActorAbilitiesComponent->PlayerSpells;
//		for (int i = 0; i < Spells.Num(); i++)
//		{
//			auto ability = Cast<AAbility>(Spells[i]->GetDefaultObject());
//			SpellsMaxUpgrade.Add(ability->AbilityMaxLevel);
//
//		}
//	}
//}




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


void AMyPlayerState::NetMulticast_SpellsUpgrade_Implementation(int index)
{
	SpellsUpgrade[index]++;
	SkillPoints = SkillPoints-1;
	//현재 플레이어의 State와 같으면
	if (this == (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerState<AMyPlayerState>()))
	{
		OnSkillPointChangedDelegate.Broadcast();
	}
}

bool AMyPlayerState::Server_SpellsUpgrade_Validate(int index)
{
	return true;
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


