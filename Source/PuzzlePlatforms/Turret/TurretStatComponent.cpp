// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretStatComponent.h"
#include "Turret.h"

// Sets default values for this component's properties
UTurretStatComponent::UTurretStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurretStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	// ...
	
}


// Called every frame
void UTurretStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UTurretStatComponent::SetHp(float NewHp)
{

	OnHPChanged.Broadcast();
	CurrentHp = NewHp;
}
void UTurretStatComponent::Server_SetHp_Implementation(float NewHp)
{
	if (NewHp <= 0)
	{
		auto TurretRef = Cast<ATurret>(GetOwner());
		TurretRef->NetMulticast_DestroyEffect();
	}
	else
	{
		NetMulticast_SetHp(NewHp);
	}

}

bool UTurretStatComponent::Server_SetHp_Validate(float NewHp)
{
	return true;
}

void UTurretStatComponent::NetMulticast_SetHp_Implementation(float NewHp)
{
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Client Hp %f"), NewHp);
	}
	SetHp(NewHp);
}

bool UTurretStatComponent::NetMulticast_SetHp_Validate(float NewHp)
{
	return true;
}