// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "GameFramework/GameStateBase.h"
#include "../PuzzlePlatformsCharacter.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Misc/DateTime.h"
#include "GoKartMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Math/Rotator.h"

// Sets default values



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

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
	Ridden = false;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AGoKart::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AGoKart::OnOverlapEnd);
}

void AGoKart::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//주변에 들어오면 차에 타게 일단 하기
	if (Cast<APuzzlePlatformsCharacter>(OtherActor) == nullptr)
		return;
	if (Ridden == false)
	{
		//OtherActor->SetActorHiddenInGame(true);
		////OtherActor->SetHidden(true);
		//Cast<APawn>(OtherActor)->GetController()->Possess(this);
		//Ridden = true;

	}
}

void AGoKart::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}




// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	

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

	if (Rider != nullptr&&Cast<APuzzlePlatformsCharacter>(Rider) != nullptr)
	{
		//+ GetActorUpVector()*50
		Cast<APuzzlePlatformsCharacter>(Rider)->DisableActor(false);
		FVector Place = this->GetTransform().GetLocation() +200*(GetActorForwardVector().RotateAngleAxis(270.0, GetActorUpVector())) ;
		Rider->SetActorLocation(Place);

		//OtherActor->SetHidden(true);
		GetController()->Possess(Rider);

	}
}

