// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY()
		int32 PlayerLevel = 1;
	//FText PlayerName;

};
