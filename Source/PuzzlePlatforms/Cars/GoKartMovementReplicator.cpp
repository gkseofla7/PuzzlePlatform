// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementReplicator.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Misc/DateTime.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

// Sets default values for this component's properties
UGoKartMovementReplicator::UGoKartMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


FString GetEnumText_(ENetRole Role)
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

void UGoKartMovementReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGoKartMovementReplicator, ServerState);

}

// Called when the game starts
void UGoKartMovementReplicator::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	//OurMovementComponent = GetOwner()->FindComponentByClass<UGoKartMovementComponent>();
	OurMovementComponent = GetOwner()->FindComponentByClass<UUpgradeMovementComponent>();
	// 지렸다;;ㅋㅋ

}



// Called every frame
void UGoKartMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FGoKartMove LastMove = OurMovementComponent->GetLastMove();
	if (GetOwnerRole() == ROLE_AutonomousProxy)//일단 서버 아니고 자기꺼 있을때 자기꺼 움직이고 서버한테 정보보냄
	{
		AddToUnacknowledgeMoves(LastMove);
		Server_SendMove(LastMove);//일단 서버한테보냄(서버가 다시 모두에게 보내게)
	}
	//We are the server and in control of the pawn
	if ((OurMovementComponent->ItsMe == true && GetOwnerRole() == ROLE_Authority)
		|| (GetOwnerRole() == ROLE_Authority && (Cast<APawn>(GetOwner()))->IsLocallyControlled()))//서버고 자기꺼일때 
	{
		UpdateServerState(LastMove);
	}

	//서버고 아무도 안타고 자기꺼 아닐때는?
	if ((OurMovementComponent->riden == false && GetOwnerRole() == ROLE_Authority)
		 && (Cast<APawn>(GetOwner()))->IsLocallyControlled())//서버고 자기꺼일때 
	{
		UpdateServerState(LastMove);
	}


	if (OurMovementComponent->ItsMe == false&&GetOwnerRole() == ROLE_SimulatedProxy)//누구든 자기꺼 아닐때
	{

		ClientTick(DeltaTime);
	}
}
void UGoKartMovementReplicator::UpdateServerState(FGoKartMove Move)
{
	//여기서 
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetTransform();
	ServerState.Velocity = OurMovementComponent->GetVelocity();
}

void UGoKartMovementReplicator::ClientTick(float DeltaTime)
{

	ClientTimeSinceUpdate += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("Back"));
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (OurMovementComponent == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Delta %f"), DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Total %f"), ClientTimeBetweenLastUpdates);
	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	
	if (LerpRatio > 1)
		LerpRatio = 1;

	FHermitCubicSpline Spline = CreateSpline();

	InterpolateLocation(Spline, LerpRatio);
	InterpolateVelocity(Spline, LerpRatio);
	InterpolateRotation(LerpRatio);

}


FHermitCubicSpline UGoKartMovementReplicator::CreateSpline()
{
	FHermitCubicSpline Spline;
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDeriavative;

	Spline.TargetLocatioin = ServerState.Transform.GetLocation();

	Spline.TargetDerivative = ServerState.Velocity * VelocityToDeriavative;
	//UE_LOG(LogTemp, Warning, TEXT("Start Velocity %f, %f, %f"), Spline.StartDerivative.X, Spline.StartDerivative.Y, Spline.StartDerivative.Z);
	//UE_LOG(LogTemp, Warning, TEXT("End Velocity %f, %f, %f"), Spline.TargetDerivative.X, Spline.TargetDerivative.Y, Spline.TargetDerivative.Z);
	return Spline;
}

void UGoKartMovementReplicator::InterpolateLocation(FHermitCubicSpline Spline, float LerpRatio)
{
	FVector NextLocation = Spline.InterpolateLocation(LerpRatio);


	FVector LocalNextLocation = GetOwner()->GetActorTransform().InverseTransformPosition(NextLocation);
	MeshOffsetRoot->SetRelativeLocation(LocalNextLocation);
	//GetOwner()->SetActorLocation(NextLocation);

	//if (MeshOffsetRoot != nullptr)
	//{
	//	MeshOffsetRoot->SetWorldLocation(NextLocation);
	//}
}

void UGoKartMovementReplicator::InterpolateVelocity(FHermitCubicSpline Spline, float LerpRatio)
{
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDeriavative;
	OurMovementComponent->SetVelocity(NewVelocity);
}

void UGoKartMovementReplicator::InterpolateRotation(float LerpRatio)
{
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat NextRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	FQuat LocalNextRotation = GetOwner()->GetActorTransform().InverseTransformRotation(NextRotation);
	//GetOwner()->SetActorRotation(NextRotation);
	MeshOffsetRoot->SetRelativeRotation(LocalNextRotation);

	//if (MeshOffsetRoot != nullptr)
	//{
	//	MeshOffsetRoot->SetWorldRotation(NextRotation);
	//}
}
void UGoKartMovementReplicator::ClearAcknowledgedMoves(FGoKartMove LastMove)
{
	TArray<FGoKartMove> NewMoves;

	for (const FGoKartMove& Move : UnacknowledgeMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgeMoves = NewMoves;

}

void UGoKartMovementReplicator::Server_SendMove_Implementation(FGoKartMove Move)
{//서버에게 부탁, 서버는 바로바로 실행
	if (OurMovementComponent == nullptr)
		return;
	ClientSimulatedTime += Move.DeltaTime;

	OurMovementComponent->SimulateMove(Move);
	UpdateServerState(Move);

}

bool UGoKartMovementReplicator::Server_SendMove_Validate(FGoKartMove Move)
{
	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;
	if (!ClientNotRunningAhead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client is running too fast"));
		return false;
	}
	if (!Move.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Recieve Invalid Move"));
		return false;
	}
	return true;

}


void UGoKartMovementReplicator::OnRep_ServerState()//약간 모두한테 실행되는듯?
{//State가 바뀌면 자동 모두에게 실행됨

	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		//UE_LOG(LogTemp, Warning, TEXT("On Rep ServerState %f *************************"), ClientTimeSinceUpdate);
		SimulatedProxy_OnRep_ServerState();

		break;
	default:
		break;
	}

}

void UGoKartMovementReplicator::SimulatedProxy_OnRep_ServerState()
{
	//아니 바뀌면 실행되는게 아니라 그냥 계속 주기적으로 실행되는거였어..?
	//UE_LOG(LogTemp, Warning, TEXT("Here is Simulate Update"));
	if (OurMovementComponent == nullptr) return;
	if (ClientTimeBetweenLastUpdates > ClientTimeSinceUpdate)//총시간에 도달못하면
	{
		float tmp = ClientTimeBetweenLastUpdates - ClientTimeSinceUpdate;
		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate + tmp;
		ClientTimeSinceUpdate = 0;
	}
	else
	{
		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
		ClientTimeSinceUpdate = 0;
	}


	//ClientStartTransform = GetOwner()->GetActorTransform();
	if (MeshOffsetRoot != nullptr)
	{

	
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		//UE_LOG(LogTemp, Warning, TEXT("Start Location %f, %f, %f"), ClientStartTransform.GetLocation().X, ClientStartTransform.GetLocation().Y, ClientStartTransform.GetLocation().Z);
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("WTF"));
	}
	ClientStartVelocity = OurMovementComponent->GetVelocity();
	GetOwner()->SetActorTransform(ServerState.Transform);
	
	//UE_LOG(LogTemp, Warning, TEXT("End Location %f, %f, %f"), ServerState.Transform.GetLocation().X, ServerState.Transform.GetLocation().Y, ServerState.Transform.GetLocation().Z);
	
}

void UGoKartMovementReplicator::AutonomousProxy_OnRep_ServerState()
{
	if (OurMovementComponent == nullptr)
		return;
	GetOwner()->SetActorTransform(ServerState.Transform);//다시 돌리는거 아니냐..?
	OurMovementComponent->SetVelocity(ServerState.Velocity);
	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FGoKartMove& Move : UnacknowledgeMoves)//결국엔 클라에서 나오는거였냐..ㅋ
	{
		//DrawDebugString(GetWorld(), FVector(0, 0, 120), GetEnumText_(GetOwner()->GetLocalRole()), GetOwner(), FColor::White, ServerState.LastMove.DeltaTime);
		OurMovementComponent->SimulateMove(Move);
	}
}