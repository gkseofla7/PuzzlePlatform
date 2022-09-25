// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PuzzlePlatforms.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMyPlayerState();

	int32 GetGameScore() const;
	void BeginPlay() override;
	void InitPlayerData();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetPlayerName(const FText& NewPlayerName);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetPlayerLevel(const int32& NewLevel);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetPlayerLevel(const int32& NewLeve);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_InitializeCharacterStat();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_InitializeCharacterStat();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SpellsUpgrade(int index);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SpellsUpgrade(int index);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetSkillPoints(int NewSkillPoint);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetSkillPoints(int NewSkillPoint);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetExp(int NewExp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetExp(int NewExp);

public:
	class UMyCharacterStatComponent* CharacterStat;
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY()
		int32 PlayerLevel = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int> SpellsUpgrade;
	int SkillPoints = 3;
	int Exp = 0;

	//class UMyCharacterStatComponent* CharacterStat;
	
	//FText PlayerName;

};
