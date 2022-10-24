// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor_Target.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMeteor_Target : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor_Target();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UDecalComponent* DecalComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USceneComponent* SceneComponent;

};
