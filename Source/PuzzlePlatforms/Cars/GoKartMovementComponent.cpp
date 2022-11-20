// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementComponent.h"

#include "GameFramework/GameStateBase.h"

// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FGoKartMove Move =CreateMove(DeltaTime);
	//SimulateMove(Move);

	if (GetOwnerRole() == ROLE_AutonomousProxy)//일단 서버 아니고 자기꺼 있을때 자기꺼 움직이고 서버한테 정보보냄
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
	//We are the server and in control of the pawn
	if (GetOwnerRole() == ROLE_Authority && (Cast<APawn>(GetOwner()))->IsLocallyControlled())//서버고 자기꺼일때 
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}

}

//void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
//	{
//		LastMove = CreateMove(DeltaTime);
//		SimulateMove(LastMove);
//	}
//}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove& Move)//애초에 서버에서만 실행됨
{

	UpdateForce(Move.Throttle);
	FVector Acceleration = TotalForce / Mass;

	float DeltaTime = Move.DeltaTime;
	Velocity += Acceleration * DeltaTime;

	ApplyRotation(DeltaTime, Move.SteeringThrow);
	UpdatesLocationFromVelocity(DeltaTime);

}

FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime)
{
	if (riden == false)
	{
		Throttle = 0;
		SteeringThrow = 0;
	}
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Throttle = Throttle;
	Move.SteeringThrow = SteeringThrow;
	Move.Time = GetOwner()->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	return Move;
}

void UGoKartMovementComponent::UpdateForce(float _Throttle)
{
	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	FVector RollingResistance = -accelerationDueToGravity * Mass * RollingCoefficient * Velocity.GetSafeNormal();
	FVector AirResistance = -Velocity.SizeSquared() * DragCoefficient * Velocity.GetSafeNormal();
	FVector Force = MaxDrivingForce * _Throttle * GetOwner()->GetActorForwardVector();

	TotalForce = Force + AirResistance + RollingResistance;

}
void UGoKartMovementComponent::ApplyRotation(float DeltaTime, float _SteeringThrow)
{
	float dx = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = dx / MinTurningRadius * _SteeringThrow;

	//float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat  RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);
	//Velocity = RotationDelta * Velocity;
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UGoKartMovementComponent::UpdatesLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;
	//Sweep이 true여야 collision
	FHitResult Result;
	GetOwner()->AddActorWorldOffset(Translation, true, &Result);

	if (Result.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}