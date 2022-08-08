// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionInterfaceComponent.h"

// Sets default values for this component's properties
UMotionInterfaceComponent::UMotionInterfaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMotionInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UMotionInterfaceComponent::Server_SendRide(AActor* _Car, APawn* _Rider)
{

}

void UMotionInterfaceComponent::Server_SendAttack()
{

}
void UMotionInterfaceComponent::DisableActor(bool toHide)
{

}