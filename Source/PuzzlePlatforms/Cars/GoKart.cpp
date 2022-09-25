// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "GameFramework/GameStateBase.h"
#include "../Character_Master.h"
#include "../PlayersComponent/PlayersMotionReplicator.h"
#include "../PlayersComponent/MotionInterfaceComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Misc/DateTime.h"
#include "GoKartMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Math/Rotator.h"
#include "AIController.h"

// Sets default values
FString GetEnumText2(ENetRole Role)
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

AGoKart::AGoKart()
{

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(false);
	//OurMovementComponent__ = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("CustomMovement_Component"));
	OurMovementComponent__ = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("UpgradeMovementComponent"));
	if (OurMovementComponent__ == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Null PTR"));
	MovementReplicator = CreateDefaultSubobject<UGoKartMovementReplicator>(TEXT("Movement_Replicator"));
	//AddOwnedComponent(OurMovementComponent);
	

	//TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	//if (!ensure(TriggerVolume != nullptr)) return;

	////auto BoxComponent = GetOwner()->FindComponentByClass<USceneComponent>();
	////TriggerVolume->SetupAttachment(BoxComponent);
	//
	//TriggerVolume->SetCollisionProfileName(TEXT("Vehicle"));
}

void AGoKart::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGoKart, Rider);
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
	//TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AGoKart::OnOverlapBegin);
	//TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AGoKart::OnOverlapEnd);
}





// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0, 0, 150), GetEnumText2(GetLocalRole()), this, FColor::White, DeltaTime);



}


// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
	PlayerInputComponent->BindAction("GetInTheCar", IE_Pressed, this, &AGoKart::GetOutTheCar);
}
void AGoKart::MoveForward(float Value)
{
	OurMovementComponent__->SetThrottle(Value);

}
void AGoKart::MoveRight(float Value)
{
	OurMovementComponent__->SetSteeringThrow(Value);
}

void AGoKart::GetOutTheCar()
{
	OurMovementComponent__->SetThrottle(0);
	OurMovementComponent__->SetSteeringThrow(0);
	Server_SendGetOutTheCar();
	OurMovementComponent__->ItsMe = true;
	OurMovementComponent__->riden = false;

}

void AGoKart::Server_SendGetOutTheCar_Implementation()
{
	if (Rider != nullptr && Cast<ACharacter_Master>(Rider) != nullptr)
	{

		//+ GetActorUpVector()*50
		Cast<UMotionInterfaceComponent>(Cast<ACharacter_Master>(Rider)->ReplicateComponent)->DisableActor(false);
		//Cast<UPlayersMotionReplicator>(Cast<ACharacter_Master>(Rider)->ReplicateComponent.GetObjectRef())->DisableActor(false);
		FVector Place = GetTransform().GetLocation() + 200 * (GetActorForwardVector().RotateAngleAxis(270.0, GetActorUpVector()));
		Rider->SetActorLocation(Place);

		//OtherActor->SetHidden(true);
		GetController()->Possess(Rider);
		//컨트롤러가 아예 없어져서;;ㅋㅋ
		if (AIController == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Controller"));
		}
		AIController->Possess(this);


		
	}
}

bool AGoKart::Server_SendGetOutTheCar_Validate()
{
	return true;
}