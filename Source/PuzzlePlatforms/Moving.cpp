// Fill out your copyright notice in the Description page of Project Settings.


#include "Moving.h"

// Sets default values
AMoving::AMoving()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void AMoving::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoving::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		AddActorWorldOffset(FVector(0.f, 0.f, 0.1f));
		//SetActorRelativeLocation(FVector(0.f, 0.f, 0.01f));
	}

}

