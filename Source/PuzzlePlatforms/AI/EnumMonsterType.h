// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumMonsterType.generated.h"


UENUM(BlueprintType)
enum class EMonsterEnum : uint8
{
	TE_Goblin UMETA(DisplayName = "Goblin"),
	TE_Archer UMETA(DisplayName = "Archer"),
	TE_IggyScorch UMETA(DisplayName = "IggyScorch"),
};


UCLASS()
class PUZZLEPLATFORMS_API AEnumMonsterType : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnumMonsterType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
