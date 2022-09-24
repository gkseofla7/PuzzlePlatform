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

// Called when the game starts
void UTargetMissileReplicateComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	OurMovementComponent = GetOwner()->FindComponentByClass<UTargetMissileMovementComponent>();
	
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());

	MeshOffsetRoot = Missile->MeshOffsetRoot;

}


// Called every frame
void UTargetMissileReplicateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());
	if (Missile->bActive == false)//쏠때 움직임
		return;

	//if (GetOwnerRole() == ROLE_AutonomousProxy)//일단 서버 아니고 자기꺼 있을때 자기꺼 움직이고 서버한테 정보보냄
	//{//즉 IsLocallyControlled() && !HasAuthority()
	//	AddToUnacknowledgeMoves(LastMove);//
	//	Server_SendMove(LastMove);//일단 서버한테보냄(서버가 다시 모두에게 보내게)
	//	//아니 걍 서버에서 바꾸고 하는게 훨 나음
	//}
	//We are the server and in control of the pawn
//	if ((GetOwnerRole() == ROLE_Authority && (Cast<APawn>(GetOwner()))->IsLocallyControlled()))//서버고 자기꺼일때 
	//{
		//UpdateServerState(LastMove);
		//Server_SendMove(LastMove);
	//}
	////서버고 아무도 안타고 자기꺼 아닐때는?
//if ((OurMovementComponent->riden == false && GetOwnerRole() == ROLE_Authority)
//	&& (Cast<APawn>(GetOwner()))->IsLocallyControlled())//서버고 자기꺼일때 
//{
//	UpdateServerState(LastMove);
//}

	//if (GetOwnerRole() == ROLE_SimulatedProxy)//누구든 자기꺼 아닐때
	//{

	//	ClientTick(DeltaTime);
	//}

	if (GetOwnerRole() == ROLE_Authority)
	{
		FTargetMissileMove LastMove = OurMovementComponent->GetLastMove();
		Server_SendMove(LastMove);
	}
	if (!GetOwner()->HasAuthority())
	{
		ClientTick(DeltaTime);
	}


}

void UTargetMissileReplicateComponent::Server_SendMove_Implementation(FTargetMissileMove Move)
{
	if (OurMovementComponent == nullptr)
		return;
	ClientSimulatedTime += Move.DeltaTime;//다른 Client를 실행시켜줄 시간

	//OurMovementComponent->SimulateMove(Move);//나 실행, 이때 이동 각도 보내줘야되는거아닌지,,ㅋㅋ
	UpdateServerState(Move);

}

bool UTargetMissileReplicateComponent::Server_SendMove_Validate(FTargetMissileMove Move)
{
	//float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	//bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;
	//if (!ClientNotRunningAhead)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Client is running too fast"));
	//	return false;
	//}
	return true;

}

void UTargetMissileReplicateComponent::UpdateServerState(FTargetMissileMove Move)
{
	//지속적으로 바뀜
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetTransform();
	ServerState.Velocity = OurMovementComponent->GetVelocity();
	//auto Loc = ServerState.Transform.GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Location %f, %f, %f"), Loc.X, Loc.Y, Loc.Z);
}

void UTargetMissileReplicateComponent::ClientTick(float DeltaTime)
{

	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (OurMovementComponent == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Total %f"), ClientTimeBetweenLastUpdates);
	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	//UE_LOG(LogTemp, Warning, TEXT("LerpRatio %f"), LerpRatio);
	if (LerpRatio > 1)
		LerpRatio = 1;

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

	FVector LocalNextLocation = GetOwner()->GetActorTransform().InverseTransformPosition(NextLocation);
	MeshOffsetRoot->SetRelativeLocation(LocalNextLocation);//Mesh만 움직여서 훼이크
	//GetOwner()->SetActorLocation(NextLocation);
	//if (MeshOffsetRoot != nullptr)
	//{
	//	MeshOffsetRoot->SetWorldLocation(NextLocation);
	//}
}

void UTargetMissileReplicateComponent::InterpolateVelocity(FHermitCubicSplines Spline, float LerpRatio)
{
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDeriavative;
	OurMovementComponent->SetVelocity(NewVelocity);
}

void UTargetMissileReplicateComponent::InterpolateRotation(float LerpRatio)
{
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat NextRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	FQuat LocalNextRotation = GetOwner()->GetActorTransform().InverseTransformRotation(NextRotation);
	//GetOwner()->SetActorRotation(NextRotation);
	MeshOffsetRoot->SetRelativeRotation(LocalNextRotation);//자기 자신만 바꿈
	//if (MeshOffsetRoot != nullptr)
	//{
	//	MeshOffsetRoot->SetWorldRotation(NextRotation);
	//}
}

void UTargetMissileReplicateComponent::OnRep_ServerState()//약간 모두한테 실행되는듯?
{
	if (!GetOwner()->HasAuthority())
	{

		SimulatedProxy_OnRep_ServerState();
	}

}

void UTargetMissileReplicateComponent::SimulatedProxy_OnRep_ServerState()
{
	//아니 바뀌면 실행되는게 아니라 그냥 계속 주기적으로 실행되는거였어..?
	//UE_LOG(LogTemp, Warning, TEXT("Here is Simulate Update"));
	if (OurMovementComponent == nullptr) return;

	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	if (MeshOffsetRoot != nullptr)
	{
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	ClientStartVelocity = OurMovementComponent->GetVelocity();
	GetOwner()->SetActorTransform(ServerState.Transform);

	//if (ClientTimeBetweenLastUpdates > ClientTimeSinceUpdate)//총시간에 도달못하면
	//{
	//	float tmp = ClientTimeBetweenLastUpdates - ClientTimeSinceUpdate;
	//	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate + tmp;//마무리 못했으니 시간 더 넣어서 시킴
	//	ClientTimeSinceUpdate = 0;
	//}
	//else//다끝냈으면 
	//{
	//	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;//여태 움직인 시간 넣음
	//	ClientTimeSinceUpdate = 0;
	//}
	//ClientStartVelocity = OurMovementComponent->GetVelocity();
	//GetOwner()->SetActorTransform(ServerState.Transform);//서버 위치가 내 위치 일단 Collision을 위한거 먼저 보내고 Mesh는 냅둠



}

void UTargetMissileReplicateComponent::AutonomousProxy_OnRep_ServerState()
{
	if (OurMovementComponent == nullptr)
		return;
	GetOwner()->SetActorTransform(ServerState.Transform);//다시 돌리는거 아니냐..?
	OurMovementComponent->SetVelocity(ServerState.Velocity);
	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FTargetMissileMove& Move : UnacknowledgeMoves)//결국엔 클라에서 나오는거였냐..ㅋ
	{
		//DrawDebugString(GetWorld(), FVector(0, 0, 120), GetEnumText_(GetOwner()->GetLocalRole()), GetOwner(), FColor::White, ServerState.LastMove.DeltaTime);
		OurMovementComponent->SimulateMove(Move);//서버에서 실행한거 이후에 다시 계산
	}
}

void UTargetMissileReplicateComponent::ClearAcknowledgedMoves(FTargetMissileMove LastMove)
{
	TArray<FTargetMissileMove> NewMoves;

	for (const FTargetMissileMove& Move : UnacknowledgeMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgeMoves = NewMoves;

}