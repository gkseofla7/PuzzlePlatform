// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "GameFramework/Actor.h"
#include "CharDamageText.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API ACharDamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharDamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetDamageText(float NewDamage);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector StartPos;

};
