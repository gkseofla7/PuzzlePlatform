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
	if (bActivate == false)//�� ������
		return;
	if (GetOwnerRole() == ROLE_AutonomousProxy)//�ϴ� ���� �ƴϰ� �ڱⲨ ������ �ڱⲨ �����̰� �������� ��������
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
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
	Move.Time = GetOwner()->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	return Move;
}

void UTargetMissileMovementComponent::SimulateMove(const FTargetMissileMove& Move)//���ʿ� ���������� �����
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
	float RotationAngle = dx / MinTurningRadius;//�ִ밢��
	auto DirectionToTarget = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), Target->GetActorLocation());
	
	auto NormalAxis = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	//�޼���ǥ��., �ݽð������ ���� ��?
	//float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	
	auto DotValue = FVector::DotProduct(GetOwner()->GetActorForwardVector(), DirectionToTarget);
	float TargetDegree = UKismetMathLibrary::DegAcos(DotValue);//���� ���ߵǴ� ���� ����
	UE_LOG(LogTemp, Warning, TEXT("Rotation Angle : %f"), RotationAngle);
	UE_LOG(LogTemp, Warning, TEXT("Target Angle : %f"), TargetDegree);
	if (TargetDegree * RotationAngle < 0)//������ ��ȣ�� �ٸ�
	{
		RotationAngle = -RotationAngle;
	}

	if (abs(TargetDegree )< abs(RotationAngle))//ȸ�������� ��ǥ������ ��ũ��
	{
		RotationAngle = TargetDegree;
	}
	if (abs(TargetDegree) > TargetingConeAngle)//������ �Ѿ������
		return;//ȸ�� ���ϰ� ����
	
	
	FQuat  RotationDelta(NormalAxis, RotationAngle);//���� Axis�� ���ؼ� ȸ����Ŵ

	Velocity = RotationDelta.RotateVector(Velocity);
	//Velocity = RotationDelta * Velocity;
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