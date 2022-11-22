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
	if (Missile->bActive == false)//쏠때 움직임
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
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;//단위를 미터로 바꿔줌, 괜히 바꿔준듯 더 헷갈림
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDeriavative;

	Spline.TargetLocatioin = ServerState.Transform.GetLocation();

	Spline.TargetDerivative = ServerState.Velocity * VelocityToDeriavative;

	
	return Spline;
}

void UTargetMissileReplicateComponent::InterpolateLocation(FHermitCubicSplines Spline, float LerpRatio)
{
	FVector NextLocation = Spline.InterpolateLocation(LerpRatio);
	
	MeshOffsetRoot->SetWorldLocation(NextLocation);//Mesh만 움직여서 훼이크

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

void UTargetMissileReplicateComponent::OnRep_ServerState()//약간 모두한테 실행되는듯?
{
	if (!GetOwner()->HasAuthority())//서버가 주인, 모든 다른 애들은 다 Simulated
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
//	GetOwner()->SetActorTransform(ServerState.Transform);//다시 돌리는거 아니냐..?
//	OurMovementComponent->SetVelocity(ServerState.Velocity);
//	ClearAcknowledgedMoves(ServerState.LastMove);
//
//	for (const FTargetMissileMove& Move : UnacknowledgeMoves)//결국엔 클라에서 나오는거였냐..ㅋ
//	{
//		//DrawDebugString(GetWorld(), FVector(0, 0, 120), GetEnumText_(GetOwner()->GetLocalRole()), GetOwner(), FColor::White, ServerState.LastMove.DeltaTime);
//		OurMovementComponent->SimulateMove(Move);//서버에서 실행한거 이후에 다시 계산
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