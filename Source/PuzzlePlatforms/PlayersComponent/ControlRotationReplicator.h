// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Soldier.h"
#include "ControlRotationReplicator.generated.h"

USTRUCT()
struct FControllerState
{
	GENERATED_BODY()
	UPROPERTY()
	FControlRotation LastControlRotation;
	UPROPERTY()
		float Pitch;
	UPROPERTY()
		float Yaw;
	UPROPERTY()
		float Time;
	UPROPERTY()
	FRotator ControlRotator;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UControlRotationReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UControlRotationReplicator();
	void ClientTick(float DeltaTime);
	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void AddToUnacknowledgeControlRotation(FControlRotation ControlRotation) { UnacknowledgeControlRotations.Add(ControlRotation); }
	void ClearAcknowledgeControlRotations(FControlRotation LastControlRotation);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateServerState(FControlRotation ControlRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendControlRotation(FControlRotation ControlRotation);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FControllerState ServerState;
	TArray<struct FControlRotation> UnacknowledgeControlRotations;
	float ClientTimeSinceUpdate;//어디까지했는지
	float ClientTimeBetweenLastUpdates;//총범위
	FRotator StartControllerRotator;
	FRotator AimRotation;

};
