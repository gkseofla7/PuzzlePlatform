// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetMissileMovementComponent.generated.h"


USTRUCT()
struct FTargetMissileMove
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
		float DeltaTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UTargetMissileMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetMissileMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FTargetMissileMove& Move);
	FVector GetVelocity() { return Velocity; };
	void SetVelocity(FVector Val) { Velocity = Val; };
	FTargetMissileMove GetLastMove() { return LastMove; };

	void UpdatesLocationFromVelocity(float DeltaTime);
//	FTargetMissileMove GetLastMove() { return LastMove; }
//private:
	FTargetMissileMove CreateMove(float DeltaTime);
//	FVector GetAirResistance();
//	FVector GetRollingResistance();
	void UpdateForce();//무조건 직진
	void ApplyRotation(float DeltaTime);
//	void UpdateLocationFromVelocity(float DeltaTime);

	UPROPERTY(EditAnywhere)
		float Mass = 1000;
	// The force applied to the car when the throttle is fully down (N).
	UPROPERTY(EditAnywhere)
		float MaxDrivingForce = 10000;
	// Minimum radius of the car turning circle at full lock (m).
	UPROPERTY(EditAnywhere)
		float MinTurningRadius = 5;
	// Higher means more drag.
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 30;
	// Higher means more rolling resistance.
	UPROPERTY(EditAnywhere)
		float RollingResistanceCoefficient = 0.015;
	FVector Velocity;
	FVector TotalForce;
	UPROPERTY(EditAnywhere)
		float RollingCoefficient = 0.15;

	FTargetMissileMove LastMove;


	AActor* Target;
	float TargetingConeAngle = 40;
	bool bActivate = false;;
	

		
};
