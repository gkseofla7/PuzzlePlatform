// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnSection.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API ARespawnSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FTransform GetRandomTransform();

public:
	UPROPERTY(EditAnywhere, Category = Trigger)
		class USphereComponent* RecoverySpace;

};
