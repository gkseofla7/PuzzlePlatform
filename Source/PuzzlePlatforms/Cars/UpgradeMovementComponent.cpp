// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeMovementComponent.h"


#include "GameFramework/GameStateBase.h"

UUpgradeMovementComponent::UUpgradeMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called every frame
void UUpgradeMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FGoKartMove Move =CreateMove(DeltaTime);
	//SimulateMove(Move);
	if (Velocity == FVector::ZeroVector)
		ItsMe = false;//��������
	if ((ItsMe == true) || GetOwnerRole() == ROLE_AutonomousProxy)//�ϴ� ���� �ƴϰ� �ڱⲨ ������ �ڱⲨ �����̰� �������� ��������
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
	//We are the server and in control of the pawn
	if (GetOwnerRole() == ROLE_Authority && (Cast<APawn>(GetOwner()))->IsLocallyControlled())//������ �ڱⲨ�϶� 
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}

}

void UUpgradeMovementComponent::SimulateMove(const FGoKartMove& Move)//���ʿ� ���������� �����
{

	//UpdateForce(Move.Throttle);
	SetThrottleInput(Move.Throttle);
	SetSteeringInput(Move.SteeringThrow);
	//FVector Acceleration = TotalForce / Mass;
	//FVector Acceleration = 
	
	float DeltaTime = Move.DeltaTime;
	//Velocity += Acceleration * DeltaTime;
	Velocity = AvoidanceVelocity;
	//ApplyRotation(DeltaTime, Move.SteeringThrow);
	//UpdatesLocationFromVelocity(DeltaTime);

}

FGoKartMove UUpgradeMovementComponent::CreateMove(float DeltaTime)
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

//void UUpgradeMovementComponent::UpdateForce(float _Throttle)
//{
//	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
//	FVector RollingResistance = -accelerationDueToGravity * Mass * RollingCoefficient * Velocity.GetSafeNormal();
//	FVector AirResistance = -Velocity.SizeSquared() * DragCoefficient * Velocity.GetSafeNormal();
//	FVector Force = MaxDrivingForce * _Throttle * GetOwner()->GetActorForwardVector();
//
//	TotalForce = Force + AirResistance + RollingResistance;
//
//}
//void UUpgradeMovementComponent::ApplyRotation(float DeltaTime, float _SteeringThrow)
//{
//	float dx = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
//	float RotationAngle = dx / MinTurningRadius * _SteeringThrow;
//
//	//float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
//	FQuat  RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
//
//	Velocity = RotationDelta.RotateVector(Velocity);
//	//Velocity = RotationDelta * Velocity;
//	GetOwner()->AddActorWorldRotation(RotationDelta);
//}
//
//void UUpgradeMovementComponent::UpdatesLocationFromVelocity(float DeltaTime)
//{
//	FVector Translation = Velocity * DeltaTime * 100;
//	//Sweep�� true���� collision
//	FHitResult Result;
//	GetOwner()->AddActorWorldOffset(Translation, true, &Result);
//
//	if (Result.IsValidBlockingHit())
//	{
//		Velocity = FVector::ZeroVector;
//	}
//}