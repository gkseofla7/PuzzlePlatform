// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "MonstersData.generated.h"

USTRUCT(BlueprintType)
struct FMonsterData : public FTableRowBase
{

	GENERATED_BODY()

public:
	FMonsterData() : Level(1), MaxHP(100.f), Attack(10.0), DropExp(10)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;


};

UCLASS()
class PUZZLEPLATFORMS_API AMonstersData : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonstersData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
