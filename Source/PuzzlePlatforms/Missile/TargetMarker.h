// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetMarker.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API ATargetMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetMarker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void LockedOn();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UWidgetComponent* WidgetComponent;
	bool bIsLockOn = false;
};
