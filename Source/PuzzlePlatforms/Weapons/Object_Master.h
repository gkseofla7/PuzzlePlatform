// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Object_Master.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AObject_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObject_Master();
	void AttachToPlayer(class APuzzlePlatformsCharacter* NewSoldier, FName SocketName);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USkeletalMeshComponent * SkeletalMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class USkeletalMeshComponent* GetSkeletalMesh() {	return SkeletalMeshComponent;}

	UPROPERTY()
		class APuzzlePlatformsCharacter* Player;
};
