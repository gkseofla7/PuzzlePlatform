// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulItem.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API ASoulItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoulItem();
	UPROPERTY(EditAnywhere ,BlueprintReadWrite)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* ParticleComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	void CustomInitialize(int exp);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION( BlueprintCallable)
	void FinishCollectItem();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialActorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACharacter_Master* PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ANPC_Master* MonsterRef;

	int Exp;

};
