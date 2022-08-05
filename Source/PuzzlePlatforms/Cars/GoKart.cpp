// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "GameFramework/GameStateBase.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PlayersComponent/PlayersMotionReplicator.h"

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
	OurMovementComponent_ = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("CustomMovement_Component"));
	MovementReplicator = CreateDefaultSubobject<UGoKartMovementReplicator>(TEXT("Movement_Replicator"));
	//AddOwnedComponent(OurMovementComponent);
	

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;

	//auto BoxComponent = GetOwner()->FindComponentByClass<USceneComponent>();
	//TriggerVolume->SetupAttachment(BoxComponent);

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
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AGoKart::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AGoKart::OnOverlapEnd);
}

void AGoKart::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//주변에 들어오면 차에 타게 일단 하기
	if (Cast<APuzzlePlatformsCharacter>(OtherActor) == nullptr)
		return;

}

void AGoKart::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

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
	OurMovementComponent_->SetThrottle(Value);

}
void AGoKart::MoveRight(float Value)
{
	OurMovementComponent_->SetSteeringThrow(Value);
}

void AGoKart::GetOutTheCar()
{
	OurMovementComponent_->SetThrottle(0);
	OurMovementComponent_->SetSteeringThrow(0);
	Server_SendGetOutTheCar();
	OurMovementComponent_->ItsMe = true;
	OurMovementComponent_->riden = false;

}

void AGoKart::Server_SendGetOutTheCar_Implementation()
{
	if (Rider != nullptr && Cast<APuzzlePlatformsCharacter>(Rider) != nullptr)
	{

		//+ GetActorUpVector()*50
		Cast<APuzzlePlatformsCharacter>(Rider)->MotionReplicator->DisableActor(false);
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