// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerData.h"

// Sets default values
AMyPlayerData::AMyPlayerData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPlayerData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayerData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

