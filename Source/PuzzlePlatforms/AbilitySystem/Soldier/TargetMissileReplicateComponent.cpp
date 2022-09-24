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
	if (Missile->bActive == false)//�� ������
		return;

	//if (GetOwnerRole() == ROLE_AutonomousProxy)//�ϴ� ���� �ƴϰ� �ڱⲨ ������ �ڱⲨ �����̰� �������� ��������
	//{//�� IsLocallyControlled() && !HasAuthority()
	//	AddToUnacknowledgeMoves(LastMove);//
	//	Server_SendMove(LastMove);//�ϴ� �������׺���(������ �ٽ� ��ο��� ������)
	//	//�ƴ� �� �������� �ٲٰ� �ϴ°� �� ����
	//}
	//We are the server and in control of the pawn
//	if ((GetOwnerRole() == ROLE_Authority && (Cast<APawn>(GetOwner()))->IsLocallyControlled()))//������ �ڱⲨ�϶� 
	//{
		//UpdateServerState(LastMove);
		//Server_SendMove(LastMove);
	//}
	////������ �ƹ��� ��Ÿ�� �ڱⲨ �ƴҶ���?
//if ((OurMovementComponent->riden == false && GetOwnerRole() == ROLE_Authority)
//	&& (Cast<APawn>(GetOwner()))->IsLocallyControlled())//������ �ڱⲨ�϶� 
//{
//	UpdateServerState(LastMove);
//}

	//if (GetOwnerRole() == ROLE_SimulatedProxy)//������ �ڱⲨ �ƴҶ�
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
	ClientSimulatedTime += Move.DeltaTime;//�ٸ� Client�� ��������� �ð�

	//OurMovementComponent->SimulateMove(Move);//�� ����, �̶� �̵� ���� ������ߵǴ°žƴ���,,����
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
	//���������� �ٲ�
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

	FVector LocalNextLocation = GetOwner()->GetActorTransform().InverseTransformPosition(NextLocation);
	MeshOffsetRoot->SetRelativeLocation(LocalNextLocation);//Mesh�� �������� ����ũ
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
	MeshOffsetRoot->SetRelativeRotation(LocalNextRotation);//�ڱ� �ڽŸ� �ٲ�
	//if (MeshOffsetRoot != nullptr)
	//{
	//	MeshOffsetRoot->SetWorldRotation(NextRotation);
	//}
}

void UTargetMissileReplicateComponent::OnRep_ServerState()//�ణ ������� ����Ǵµ�?
{
	if (!GetOwner()->HasAuthority())
	{

		SimulatedProxy_OnRep_ServerState();
	}

}

void UTargetMissileReplicateComponent::SimulatedProxy_OnRep_ServerState()
{
	//�ƴ� �ٲ�� ����Ǵ°� �ƴ϶� �׳� ��� �ֱ������� ����Ǵ°ſ���..?
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

	//if (ClientTimeBetweenLastUpdates > ClientTimeSinceUpdate)//�ѽð��� ���޸��ϸ�
	//{
	//	float tmp = ClientTimeBetweenLastUpdates - ClientTimeSinceUpdate;
	//	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate + tmp;//������ �������� �ð� �� �־ ��Ŵ
	//	ClientTimeSinceUpdate = 0;
	//}
	//else//�ٳ������� 
	//{
	//	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;//���� ������ �ð� ����
	//	ClientTimeSinceUpdate = 0;
	//}
	//ClientStartVelocity = OurMovementComponent->GetVelocity();
	//GetOwner()->SetActorTransform(ServerState.Transform);//���� ��ġ�� �� ��ġ �ϴ� Collision�� ���Ѱ� ���� ������ Mesh�� ����



}

void UTargetMissileReplicateComponent::AutonomousProxy_OnRep_ServerState()
{
	if (OurMovementComponent == nullptr)
		return;
	GetOwner()->SetActorTransform(ServerState.Transform);//�ٽ� �����°� �ƴϳ�..?
	OurMovementComponent->SetVelocity(ServerState.Velocity);
	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FTargetMissileMove& Move : UnacknowledgeMoves)//�ᱹ�� Ŭ�󿡼� �����°ſ���..��
	{
		//DrawDebugString(GetWorld(), FVector(0, 0, 120), GetEnumText_(GetOwner()->GetLocalRole()), GetOwner(), FColor::White, ServerState.LastMove.DeltaTime);
		OurMovementComponent->SimulateMove(Move);//�������� �����Ѱ� ���Ŀ� �ٽ� ���
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