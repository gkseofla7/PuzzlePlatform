// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"


USTRUCT()
struct FGoKartMove
{
	GENERATED_BODY()

		UPROPERTY()
		float Throttle;
	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;

	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEPLATFORMS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGoKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	void SimulateMove(const FGoKartMove& Move);
	FGoKartMove CreateMove(float DeltaTime);

	void UpdateForce(float Throttle);

	void UpdatesLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void SetVelocity(FVector _Velocity) { Velocity = _Velocity; }
	FVector GetVelocity() { return Velocity; }
	void SetThrottle(float _Throttle) { Throttle = _Throttle; }
	void SetSteeringThrow(float _SteeringThrow) { SteeringThrow = _SteeringThrow; }
	FGoKartMove GetLastMove() { return LastMove; }

public:
	//Mass of the car (kg).
	UPROPERTY(EditAnywhere)
		float Mass = 1000;
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 16;
	UPROPERTY(EditAnywhere)
		float RollingCoefficient = 0.15;
	//The force applied to the car when the throttle is fully down
	UPROPERTY(EditAnywhere)
		float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)//미터로 계산
		float MinTurningRadius = 10;
private:
	FVector Velocity;
	FVector TotalForce;
	float Throttle;
	float SteeringThrow;
	FGoKartMove LastMove;

};
