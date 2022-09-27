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
	//Ŀ�ø�,,����
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());

	if (Missile->bActive == false)//�� ������
		return;
	if (GetOwnerRole() == ROLE_Authority )//�����϶�
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
	//�׳� �ڱ� �ڽ��϶� ����
}


FTargetMissileMove UTargetMissileMovementComponent::CreateMove(float DeltaTime)
{
	FTargetMissileMove Move;
	Move.DeltaTime = DeltaTime;
	return Move;
}

void UTargetMissileMovementComponent::SimulateMove(const FTargetMissileMove& Move)//���ʿ� ���������� �����
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
	//��� ȿ����
	float dx = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float MaxAngle = dx / MinTurningRadius;
	float RotationAngle = MaxAngle;//�ִ밢��
	auto DirectionToTarget = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	
	auto NormalAxis = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	auto DotValue = FVector::DotProduct(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	float TargetDegree = UKismetMathLibrary::DegAcos(DotValue);//���� ���ߵǴ� ���� ����
	if (TargetDegree * RotationAngle < 0)//������ ��ȣ�� �ٸ�
	{
		RotationAngle = -RotationAngle;
	}
	if (abs(TargetDegree )< abs(RotationAngle))//ȸ�������� ��ǥ������ ��ũ��
	{
		RotationAngle = TargetDegree;
	}
	
	FQuat  RotationDelta(NormalAxis, RotationAngle);//���� Axis�� ���ؼ� ȸ����Ŵ

	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UTargetMissileMovementComponent::UpdatesLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100;
	//Sweep�� true���� collision
	FHitResult Result;
	GetOwner()->AddActorWorldOffset(Translation, true, &Result);

	if (Result.IsValidBlockingHit())//�ε����� ����
	{
		Velocity = FVector::ZeroVector;
	}
}