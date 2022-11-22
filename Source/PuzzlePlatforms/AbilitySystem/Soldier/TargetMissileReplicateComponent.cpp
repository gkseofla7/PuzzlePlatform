// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability_TargetMissile.h"

#include "TargetMissileReplicateComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UTargetMissileReplicateComponent::UTargetMissileReplicateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	// ...
}


FString GetEnumToText(ENetRole Role)
{
	
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "Error";
	}
}

void UTargetMissileReplicateComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTargetMissileReplicateComponent, ServerState);

}

void UTargetMissileReplicateComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	OurMovementComponent = GetOwner()->FindComponentByClass<UTargetMissileMovementComponent>();
	
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());

	MeshOffsetRoot = Missile->MeshOffsetRoot;

}


void UTargetMissileReplicateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());
	if (Missile->bActive == false)//�� ������
		return;

	if (GetOwnerRole() == ROLE_Authority)
	{
		UpdateServerState();
	}
	if (!GetOwner()->HasAuthority())
	{
		ClientTick(DeltaTime);
	}


}

void UTargetMissileReplicateComponent::UpdateServerState()
{
	if (OurMovementComponent == nullptr)
		return;
	ServerState.Transform = GetOwner()->GetTransform();
	ServerState.Velocity = OurMovementComponent->GetVelocity();
}

void UTargetMissileReplicateComponent::ClientTick(float DeltaTime)
{

	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (OurMovementComponent == nullptr) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

	if (LerpRatio > 1)
	{
		LerpRatio = 1;

	}
		

	FHermitCubicSplines Spline = CreateSpline();

	InterpolateLocation(Spline, LerpRatio);
	InterpolateVelocity(Spline, LerpRatio);
	InterpolateRotation(LerpRatio);

}

FHermitCubicSplines UTargetMissileReplicateComponent::CreateSpline()
{
	FHermitCubicSplines Spline;
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;//������ ���ͷ� �ٲ���, ���� �ٲ��ص� �� �򰥸�
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDeriavative;

	Spline.TargetLocatioin = ServerState.Transform.GetLocation();

	Spline.TargetDerivative = ServerState.Velocity * VelocityToDeriavative;

	
	return Spline;
}

void UTargetMissileReplicateComponent::InterpolateLocation(FHermitCubicSplines Spline, float LerpRatio)
{
	FVector NextLocation = Spline.InterpolateLocation(LerpRatio);
	
	MeshOffsetRoot->SetWorldLocation(NextLocation);//Mesh�� �������� ����ũ

}

void UTargetMissileReplicateComponent::InterpolateVelocity(FHermitCubicSplines Spline, float LerpRatio)
{
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDeriavative;
	UE_LOG(LogTemp, Warning, TEXT("Speed %f"), NewVelocity.Size());
	OurMovementComponent->SetVelocity(NewVelocity);
}

void UTargetMissileReplicateComponent::InterpolateRotation(float LerpRatio)
{
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat NextRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldRotation(NextRotation);
	}
}

void UTargetMissileReplicateComponent::OnRep_ServerState()//�ణ ������� ����Ǵµ�?
{
	if (!GetOwner()->HasAuthority())//������ ����, ��� �ٸ� �ֵ��� �� Simulated
	{

		if (OurMovementComponent == nullptr) return;

		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
		ClientTimeSinceUpdate = 0;
		UE_LOG(LogTemp, Warning, TEXT("ClientTimeBetweenLastUpdates %f"), ClientTimeBetweenLastUpdates);
		if (MeshOffsetRoot != nullptr)
		{
			ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
			ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
		}
		ClientStartVelocity = OurMovementComponent->GetVelocity();
		GetOwner()->SetActorTransform(ServerState.Transform);
	}

}






//void UTargetMissileReplicateComponent::AutonomousProxy_OnRep_ServerState()
//{
//	if (OurMovementComponent == nullptr)
//		return;
//	GetOwner()->SetActorTransform(ServerState.Transform);//�ٽ� �����°� �ƴϳ�..?
//	OurMovementComponent->SetVelocity(ServerState.Velocity);
//	ClearAcknowledgedMoves(ServerState.LastMove);
//
//	for (const FTargetMissileMove& Move : UnacknowledgeMoves)//�ᱹ�� Ŭ�󿡼� �����°ſ���..��
//	{
//		//DrawDebugString(GetWorld(), FVector(0, 0, 120), GetEnumText_(GetOwner()->GetLocalRole()), GetOwner(), FColor::White, ServerState.LastMove.DeltaTime);
//		OurMovementComponent->SimulateMove(Move);//�������� �����Ѱ� ���Ŀ� �ٽ� ���
//	}
//}

//void UTargetMissileReplicateComponent::ClearAcknowledgedMoves(FTargetMissileMove LastMove)
//{
//	TArray<FTargetMissileMove> NewMoves;
//
//	for (const FTargetMissileMove& Move : UnacknowledgeMoves)
//	{
//		if (Move.Time > LastMove.Time)
//		{
//			NewMoves.Add(Move);
//		}
//	}
//
//	UnacknowledgeMoves = NewMoves;
//
//}