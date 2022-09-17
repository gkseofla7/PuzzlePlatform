// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSection.h"

#include "Components/SphereComponent.h"
#include "NavigationSystem.h"

// Sets default values
ARespawnSection::ARespawnSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RecoverySpace = CreateDefaultSubobject<USphereComponent>(TEXT("RecoverySpace"));
	RecoverySpace->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARespawnSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ARespawnSection::GetRandomTransform()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoNavSystem"));
		FTransform out;
		return out;
	}
	FVector Origin = GetActorLocation();
	FNavLocation NextPatrol;
	FVector RandomLoc;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500, NextPatrol))
	{
		RandomLoc = NextPatrol.Location;
		RandomLoc.Z += 90;

	}
	FTransform OutTransform;
	OutTransform.SetLocation(RandomLoc);

	return OutTransform;
}