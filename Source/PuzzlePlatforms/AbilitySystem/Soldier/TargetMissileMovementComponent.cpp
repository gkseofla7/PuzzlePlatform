// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetMissileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameStateBase.h"
// Sets default values for this component's properties
UTargetMissileMovementComponent::UTargetMissileMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetMissileMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetMissileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bActivate == false)//쏠때 움직임
		return;
	if (GetOwnerRole() == ROLE_AutonomousProxy)//일단 서버 아니고 자기꺼 있을때 자기꺼 움직이고 서버한테 정보보냄
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
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
	Move.Time = GetOwner()->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	return Move;
}

void UTargetMissileMovementComponent::SimulateMove(const FTargetMissileMove& Move)//애초에 서버에서만 실행됨
{

	UpdateForce();
	FVector Acceleration = TotalForce / Mass;

	float DeltaTime = Move.DeltaTime;
	Velocity += Acceleration * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("Velocity : %f, %f, %f"), Velocity.X, Velocity.Y, Velocity.Z);
	ApplyRotation(DeltaTime);
	UpdatesLocationFromVelocity(DeltaTime);

}


void UTargetMissileMovementComponent::UpdateForce()
{
	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	FVector RollingResistance = -accelerationDueToGravity * Mass * RollingCoefficient * Velocity.GetSafeNormal();
	FVector AirResistance = -Velocity.SizeSquared() * DragCoefficient * Velocity.GetSafeNormal();
	FVector Force = MaxDrivingForce  * GetOwner()->GetActorForwardVector();

	TotalForce = Force + AirResistance + RollingResistance;

}


void UTargetMissileMovementComponent::ApplyRotation(float DeltaTime)
{
	float dx = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = dx / MinTurningRadius;//최대각도
	auto DirectionToTarget = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	
	auto NormalAxis = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	//왼손좌표계., 반시계방향이 각도 양?
	//float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	
	auto DotValue = FVector::DotProduct(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	float TargetDegree = UKismetMathLibrary::DegAcos(DotValue);//현재 가야되는 방향 각도
	UE_LOG(LogTemp, Warning, TEXT("Rotation Angle : %f"), RotationAngle);
	UE_LOG(LogTemp, Warning, TEXT("Target Angle : %f"), TargetDegree);
	if (TargetDegree * RotationAngle < 0)//음수면 부호가 다름
	{
		RotationAngle = -RotationAngle;
	}

	if (abs(TargetDegree )< abs(RotationAngle))//회전각도가 목표각보다 더크면
	{
		RotationAngle = TargetDegree;
	}
	if (abs(TargetDegree) > TargetingConeAngle)//범위를 넘어가버리면
		return;//회전 안하고 직진
	
	
	FQuat  RotationDelta(NormalAxis, RotationAngle);//따로 Axis를 구해서 회전시킴

	Velocity = RotationDelta.RotateVector(Velocity);
	//Velocity = RotationDelta * Velocity;
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