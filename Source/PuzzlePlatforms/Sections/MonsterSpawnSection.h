// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "../AI/NPC_Master.h"
#include "MonsterSpawnSection.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMonsterSpawnSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnSection();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void OnNPCSpawn();
	


public:	
	UPROPERTY(EditAnywhere, Category = Trigger)
		class UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere)
		float EnemySpawnTime;
	UPROPERTY( EditAnywhere)
	TSubclassOf<class  ANPC_Master>NPC_MasterClass1;
	UPROPERTY(EditAnywhere)
	TSubclassOf< class ANPC_Master>NPC_MasterClass2;
	FTimerHandle SpawnNPCTimerHandle = {};
	bool Toggle = true;
	TArray<class ANPC_Master*> Monsters;
};
