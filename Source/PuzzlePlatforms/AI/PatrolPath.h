// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> PathPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsLooping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WaitTime;
};
