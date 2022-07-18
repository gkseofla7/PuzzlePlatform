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
		//�� ���������� ��ġ�� �ǵ� �� ����, Ŭ���̾�Ʈ���� �ǵ�� �ٸ��������� �ȹٲ�
		//����ȭ�� �ѹ���
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