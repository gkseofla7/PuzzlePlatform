// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "GoKartMovementComponent.h"
#include "UpgradeMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"


USTRUCT()
struct FGoKartState
{
	GENERATED_BODY()

		UPROPERTY()
		FTransform Transform;
	UPROPERTY()
		FGoKartMove LastMove;
	UPROPERTY()
		FVector Velocity;
};

struct FHermitCubicSpline
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

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEPLATFORMS_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGoKartMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetVelocityToDeriavative() { return  ClientTimeBetweenLastUpdates * 100; }


	void ClearAcknowledgedMoves(FGoKartMove LastMove);


	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FGoKartMove Value);


	UFUNCTION()
		void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();

	void AddToUnacknowledgeMoves(FGoKartMove Move) { UnacknowledgeMoves.Add(Move); }

	FGoKartState GetServerState() { return ServerState; }
	void UpdateServerState(FGoKartMove Move);
	void ClientTick(float DeltaTime);
private:




	//임시방편으로 
	//보간
	FHermitCubicSpline CreateSpline();
	void InterpolateLocation(FHermitCubicSpline Input, float LerpRatio);
	void InterpolateVelocity(FHermitCubicSpline Input, float LerpRatio);
	void InterpolateRotation(float LerpRatio);
	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
	UPROPERTY()
		USceneComponent* MeshOffsetRoot;
	float ClientSimulatedTime;
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FGoKartState ServerState;
	//UPROPERTY(VisibleAnywhere)
	//	UGoKartMovementComponent* OurMovementComponent;
	UPROPERTY(VisibleAnywhere)
		UUpgradeMovementComponent* OurMovementComponent;
	FVector ClientStartVelocity;
	float ClientTimeSinceUpdate;//어디까지했는지
	float ClientTimeBetweenLastUpdates;//총범위
	FTransform ClientStartTransform;
	TArray<struct FGoKartMove> UnacknowledgeMoves;




};
