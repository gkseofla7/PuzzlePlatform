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
	//OurMovementComponent = GetOwner()->FindComponentByClass<UGoKartMovementComponent>();
}


// Called every frame
void UTargetMissileReplicateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto Missile = Cast< AAbility_TargetMissile>(GetOwner());
	if (Missile->bActive == false)//�� ������
		return;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FTargetMissileMove LastMove = OurMovementComponent->GetLastMove();
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
		UE_LOG(LogTemp, Warning, TEXT("ServerTick"));
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
	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;
	if (!ClientNotRunningAhead)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client is running too fast"));
		return false;
	}
	return true;

}

void UTargetMissileReplicateComponent::UpdateServerState(FTargetMissileMove Move)
{
	//���������� �ٲ�
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetTransform();
	ServerState.Velocity = OurMovementComponent->GetVelocity();
}

void UTargetMissileReplicateComponent::ClientTick(float DeltaTime)
{

	ClientTimeSinceUpdate += DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("ClientTick"));
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

FHermitCubicSpline UTargetMissileReplicateComponent::CreateSpline()
{
	FHermitCubicSpline Spline;
	float VelocityToDeriavative = ClientTimeBetweenLastUpdates * 100;//������ ���ͷ� �ٲ���, ���� �ٲ��ص� �� �򰥸�
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDeriavative;

	Spline.TargetLocatioin = ServerState.Transform.GetLocation();

	Spline.TargetDerivative = ServerState.Velocity * VelocityToDeriavative;
	//UE_LOG(LogTemp, Warning, TEXT("Start Velocity %f, %f, %f"), Spline.StartDerivative.X, Spline.StartDerivative.Y, Spline.StartDerivative.Z);
	//UE_LOG(LogTemp, Warning, TEXT("End Velocity %f, %f, %f"), Spline.TargetDerivative.X, Spline.TargetDerivative.Y, Spline.TargetDerivative.Z);
	return Spline;
}

void UTargetMissileReplicateComponent::InterpolateLocation(FHermitCubicSpline Spline, float LerpRatio)
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

void UTargetMissileReplicateComponent::InterpolateVelocity(FHermitCubicSpline Spline, float LerpRatio)
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
{//State�� �ٲ�� �ڵ� ��ο��� �����
	//�� Client ��ο��� �ؾߵ�
	//switch (GetOwnerRole())
	//{
	//case ROLE_AutonomousProxy:
	//	AutonomousProxy_OnRep_ServerState();
	//	break;
	//case ROLE_SimulatedProxy:
	//	//UE_LOG(LogTemp, Warning, TEXT("On Rep ServerState %f *************************"), ClientTimeSinceUpdate);
	//	SimulatedProxy_OnRep_ServerState();
	//	break;
	//default:
	//	break;
	//}
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
	if (ClientTimeBetweenLastUpdates > ClientTimeSinceUpdate)//�ѽð��� ���޸��ϸ�
	{
		float tmp = ClientTimeBetweenLastUpdates - ClientTimeSinceUpdate;
		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate + tmp;//������ �������� �ð� �� �־ ��Ŵ
		ClientTimeSinceUpdate = 0;
	}
	else//�ٳ������� 
	{
		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;//���� ������ �ð� ����
		ClientTimeSinceUpdate = 0;
	}
	//ClientStartTransform = GetOwner()->GetActorTransform();
	if (MeshOffsetRoot != nullptr)//���� Mesh ��ġ�� �� ��ġ
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
	GetOwner()->SetActorTransform(ServerState.Transform);//���� ��ġ�� �� ��ġ �ϴ� Collision�� ���Ѱ� ���� ������ Mesh�� ����

	//UE_LOG(LogTemp, Warning, TEXT("End Location %f, %f, %f"), ServerState.Transform.GetLocation().X, ServerState.Transform.GetLocation().Y, ServerState.Transform.GetLocation().Z);

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