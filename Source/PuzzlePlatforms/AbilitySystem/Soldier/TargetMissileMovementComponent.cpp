// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetMissileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "Ability_TargetMissile.h"
UTargetMissileMovementComponent::UTargetMissileMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTargetMissileMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTargetMissileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//커플링,,ㅋㅋ
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());

	if (Missile->bActive == false)//쏠때 움직임
		return;
	if (GetOwnerRole() == ROLE_Authority )//서버일때
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
	//그냥 자기 자신일때 보냄
}


FTargetMissileMove UTargetMissileMovementComponent::CreateMove(float DeltaTime)
{
	FTargetMissileMove Move;
	Move.DeltaTime = DeltaTime;
	return Move;
}

void UTargetMissileMovementComponent::SimulateMove(const FTargetMissileMove& Move)//애초에 서버에서만 실행됨
{

	UpdateForce();
	FVector Acceleration = TotalForce / Mass;

	float DeltaTime = Move.DeltaTime;
	Velocity += Acceleration * DeltaTime;
	ApplyRotation(DeltaTime);
	UpdatesLocationFromVelocity(DeltaTime);

}


void UTargetMissileMovementComponent::UpdateForce()
{
	FVector AirResistance = -Velocity.SizeSquared() * DragCoefficient * Velocity.GetSafeNormal();
	FVector Force = MaxDrivingForce  * GetOwner()->GetActorForwardVector();

	TotalForce = Force + AirResistance;
}


void UTargetMissileMovementComponent::ApplyRotation(float DeltaTime)
{
	//계산 효율적
	float dx = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float MaxAngle = dx / MinTurningRadius;
	float RotationAngle = MaxAngle;//최대각도
	auto DirectionToTarget = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	
	auto NormalAxis = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	auto DotValue = FVector::DotProduct(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	float TargetDegree = UKismetMathLibrary::DegAcos(DotValue);//현재 가야되는 방향 각도
	if (TargetDegree * RotationAngle < 0)//음수면 부호가 다름
	{
		RotationAngle = -RotationAngle;
	}
	if (abs(TargetDegree )< abs(RotationAngle))//회전각도가 목표각보다 더크면
	{
		RotationAngle = TargetDegree;
	}
	
	FQuat  RotationDelta(NormalAxis, RotationAngle);//따로 Axis를 구해서 회전시킴

	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UTargetMissileMovementComponent::UpdatesLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;
	//Sweep이 true여야 collision
	FHitResult Result;
	GetOwner()->AddActorWorldOffset(Translation, true, &Result);

	if (Result.IsValidBlockingHit())//부딪히면 멈춤
	{
		Velocity = FVector::ZeroVector;
	}
}