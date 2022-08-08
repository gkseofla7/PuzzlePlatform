// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicleMovementComponent.h"
#include "GoKartMovementComponent.h"
#include "UpgradeMovementComponent.generated.h"



UCLASS()
class PUZZLEPLATFORMS_API UUpgradeMovementComponent : public UWheeledVehicleMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUpgradeMovementComponent();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:

	bool ItsMe = false;
	bool riden = false;
	void SimulateMove(const FGoKartMove& Move);
	FGoKartMove CreateMove(float DeltaTime);

	//void UpdateForce(float Throttle);


	//void UpdatesLocationFromVelocity(float DeltaTime);
	//void ApplyRotation(float DeltaTime, float SteeringThrow);
	void SetVelocity(FVector _Velocity) {
		Velocity = _Velocity; 
		AvoidanceVelocity = Velocity;
	}
	FVector GetVelocity() { return Velocity; }
	void SetThrottle(float _Throttle) { Throttle = _Throttle; }
	void SetSteeringThrow(float _SteeringThrow) { SteeringThrow = _SteeringThrow; }
	FGoKartMove GetLastMove() { return LastMove; }

public:
	//Mass of the car (kg).
	//UPROPERTY(EditAnywhere)
	//	float Mass = 1000;
	//UPROPERTY(EditAnywhere)
	//	float DragCoefficient = 16;
	//UPROPERTY(EditAnywhere)
	//	float RollingCoefficient = 0.15;
	//The force applied to the car when the throttle is fully down
	//UPROPERTY(EditAnywhere)
	//	float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)//미터로 계산
		float MinTurningRadius = 10;
private:
	//FVector Velocity;
	FVector TotalForce;
	float Throttle;
	float SteeringThrow;
	FGoKartMove LastMove;
};
