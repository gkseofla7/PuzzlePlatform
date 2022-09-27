// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetMissileMovementComponent.h"
#include "TargetMissileReplicateComponent.generated.h"



USTRUCT()
struct FTargetMissileState
{
	GENERATED_BODY()

		UPROPERTY()
		FTransform Transform;
	UPROPERTY()
		FTargetMissileMove LastMove;
	UPROPERTY()
		FVector Velocity;
};

struct FHermitCubicSplines
{
	FVector StartLocation, StartDerivative, TargetLocatioin, TargetDerivative;

	FVector InterpolateLocation(float LerpRatio)
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocatioin, TargetDerivative, LerpRatio);
	}
	FVector InterpolateDerivative(float LerpRatio)
	{
		return  FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocatioin, TargetDerivative, LerpRatio);
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UTargetMissileReplicateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetMissileReplicateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetVelocityToDeriavative() { return  ClientTimeBetweenLastUpdates * 100; }
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FTargetMissileMove Value);
	UFUNCTION()
		void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AddToUnacknowledgeMoves(FTargetMissileMove Move) { UnacknowledgeMoves.Add(Move); }
	FHermitCubicSplines CreateSpline();
	FTargetMissileState GetServerState() { return ServerState; }
	void UpdateServerState(FTargetMissileMove Move);
	void ClientTick(float DeltaTime);
	void InterpolateLocation(FHermitCubicSplines Input, float LerpRatio);
	void InterpolateVelocity(FHermitCubicSplines Input, float LerpRatio);
	void InterpolateRotation(float LerpRatio);

public:
	UPROPERTY()
		USceneComponent* MeshOffsetRoot;
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FTargetMissileState ServerState;//현재 서버 상태

	UPROPERTY(VisibleAnywhere)
		UTargetMissileMovementComponent* OurMovementComponent;
	FVector ClientStartVelocity;
	float ClientTimeSinceUpdate;//어디까지했는지
	float ClientTimeBetweenLastUpdates;//총범위
	FTransform ClientStartTransform;
	TArray<struct FTargetMissileMove> UnacknowledgeMoves;

	
};


