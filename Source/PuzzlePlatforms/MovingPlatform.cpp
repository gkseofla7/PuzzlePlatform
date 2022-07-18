// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetActorTransform().TransformPosition(TargetLocation);
	if (HasAuthority())
	{
		//즉 서버에서만 위치를 건들 수 있음, 클라이언트에서 건들면 다른곳에서는 안바뀜
		//동기화는 한방향
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(ActiveTriggers>0)
		ActiveTriggers--;
}

void AMovingPlatform::Tick(float Delta)
{
	Super::Tick(Delta);

	if (HasAuthority()&& ActiveTriggers>0)
	{
		
		
		float DistanceAB = (GlobalTargetLocation - GlobalStartLocation).Size();
		FVector Location = GetActorLocation();
		if (DistanceAB < (Location - GlobalStartLocation).Size())
		{
			FVector tmp = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = tmp;
		}
		FVector DirectionToGo = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		Location += DirectionToGo* Delta*Cube_Velocity;
		SetActorLocation(Location);
		
	}
	
}