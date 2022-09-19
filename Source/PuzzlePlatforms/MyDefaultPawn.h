// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "MyDefaultPawn.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMyDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
public:
	void BeginPlay();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PossessCharacter(APlayerController* NewController, int Index);
};
