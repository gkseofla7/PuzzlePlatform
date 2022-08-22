// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityDetailStruct.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AAbilityDetailStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityDetailStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic, BlueprintType)
struct FAbilityDetailsStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CastTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInterruptable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RequiresTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;
};