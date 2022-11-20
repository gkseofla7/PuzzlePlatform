// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlRotationReplicator.h"
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
UControlRotationReplicator::UControlRotationReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UControlRotationReplicator::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	// ...
	
}

void UControlRotationReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UControlRotationReplicator, ServerState);

}

// Called every frame
void UControlRotationReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ASoldier* MySoldier = Cast<ASoldier>(GetOwner());
	if (MySoldier->EquippedItem == nullptr)
		return;
	FControlRotation LastControlRotation = Cast<ASoldier>(GetOwner())->LastControlRotation;
	if (GetOwnerRole() == ROLE_AutonomousProxy)//일단 서버 아니고 자기꺼 있을때 자기꺼 움직이고 서버한테 정보보냄
	{
		AddToUnacknowledgeControlRotation(LastControlRotation);
		Server_SendControlRotation(LastControlRotation);//일단 서버한테보냄(서버가 다시 모두에게 보내게)
	}
	//We are the server and in control of the pawn
	if (GetOwnerRole() == ROLE_Authority&&Cast<ACharacter>(GetOwner())->IsLocallyControlled())//서버고 자기꺼일때 
	{
		//UE_LOG(LogTemp, Warning, TEXT("TICKCOMPONENT"));
		UpdateServerState(LastControlRotation);
	}
	if (GetOwnerRole() == ROLE_SimulatedProxy)//누구든 자기꺼 아닐때
	{
		ClientTick(DeltaTime);
	}
}

void UControlRotationReplicator::ClientTick(float DeltaTime)
{
//	UE_LOG(LogTemp, Warning, TEXT("WTFHere"));
	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

	if (LerpRatio > 1)
		LerpRatio = 1;

	AimRotation = FQuat::Slerp(StartControllerRotator.Quaternion(), ServerState.ControlRotator.Quaternion(), LerpRatio).Rotator();
	ASoldier* MySoldier = Cast<ASoldier>(GetOwner());

	//UE_LOG(LogTemp, Warning, TEXT("AimRotation : %f, %f, %f"), AimRotation.Pitch, AimRotation.Yaw, AimRotation.Roll);
	MySoldier->SimulateRotationAnimation(AimRotation);
}

void UControlRotationReplicator::UpdateServerState(FControlRotation ControlRotation)
{
	//여기서 
	ServerState.LastControlRotation = ControlRotation;
	ServerState.Pitch = ControlRotation.Pitch;
	ServerState.Yaw = ControlRotation.Yaw;
	ServerState.Time = ControlRotation.Time;

	//UE_LOG(LogTemp, Warning, TEXT("UpdateServerState : %f"), ServerState.LastControlRotation.Time);
	ServerState.ControlRotator = Cast<APawn>(GetOwner())->GetControlRotation();
	//UE_LOG(LogTemp, Warning, TEXT("UpdateServerState Rotator : %f, %f, %f"), ServerState.ControlRotator.Pitch, ServerState.ControlRotator.Yaw, ServerState.ControlRotator.Roll);
}

void UControlRotationReplicator::ClearAcknowledgeControlRotations(FControlRotation LastControlRotation)
{
	TArray<FControlRotation> NewControlRotations;
	//UE_LOG(LogTemp, Warning, TEXT("Clear Num : %d"), UnacknowledgeControlRotations.Num());
	for (const FControlRotation& ControlRotation : UnacknowledgeControlRotations)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Time %f, %f"), ControlRotation.Time, LastControlRotation.Time);
		if (ControlRotation.Time > LastControlRotation.Time)
		{
			NewControlRotations.Add(ControlRotation);
		}
	}

	UnacknowledgeControlRotations = NewControlRotations;

}

void UControlRotationReplicator::Server_SendControlRotation_Implementation(FControlRotation ControlRotation)
{//서버에게 부탁, 서버는 바로바로 실행
	//ClientSimulatedTime += Move.DeltaTime;
	ASoldier* MySoldier = Cast<ASoldier>(GetOwner());
	MySoldier->SimulateControllerRotation(ControlRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Time %f"), ControlRotation.Time);
	UpdateServerState(ControlRotation);
}

bool UControlRotationReplicator::Server_SendControlRotation_Validate(FControlRotation ControlRotation)
{
	return true;

}


void UControlRotationReplicator::OnRep_ServerState()
{
	//UE_LOG(LogTemp, Warning, TEXT("ServerState Pitch : %f"), ServerState.LastControlRotation.Pitch);
	//UE_LOG(LogTemp, Warning, TEXT("ServerState Time : %f"), ServerState.LastControlRotation.Time);
	//UE_LOG(LogTemp, Warning, TEXT("ServerStateRotator : %f, %f, %f"), ServerState.ControlRotator.Pitch, ServerState.ControlRotator.Yaw, ServerState.ControlRotator.Roll);
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRep_ServerState();
		break;
	default:
		break;
	}
}
void UControlRotationReplicator::SimulatedProxy_OnRep_ServerState()//여기서는 초기값 초기화~
{
	//auto Anim = Cast<USoldierAnimInstance>(MyAnim);
	//Anim->ClientTimeBetweenLastUpdates = Anim->ClientTimeSinceUpdate;
	//Anim->ClientTimeSinceUpdate = 0;

	//Anim->StartControllerRotator = Anim->AimRotation;

	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	StartControllerRotator = AimRotation;//여기서 초기화시켜줌~
	//UE_LOG(LogTemp, Warning, TEXT("Num : %d"), UnacknowledgeControlRotations.Num());
	//GetOwner()->SetActorTransform(ServerState.Tranform);

}
void UControlRotationReplicator::AutonomousProxy_OnRep_ServerState()
{
	ASoldier* MySoldier = Cast<ASoldier>(GetOwner());
	MySoldier->GetController()->SetControlRotation(ServerState.ControlRotator);
	FControlRotation MyControlRotation;
	MyControlRotation.Pitch = ServerState.Pitch;
	MyControlRotation.Yaw = ServerState.Yaw;
	MyControlRotation.Time = ServerState.Time;

	ClearAcknowledgeControlRotations(MyControlRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Num : %d"), UnacknowledgeControlRotations.Num());
	for (const FControlRotation& ControlRotation : UnacknowledgeControlRotations)//결국엔 클라에서 나오는거였냐..ㅋ
	{
		MySoldier->SimulateControllerRotation(ControlRotation);
	}

}