// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.h"
#include "UpgradeMovementComponent.h"
#include "GoKart.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetRider(APawn* _rider) { Rider = _rider; }

private:


	void MoveForward(float Value);
	void MoveRight(float Value);
	


public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	UGoKartMovementComponent* OurMovementComponent__;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UUpgradeMovementComponent* OurMovementComponent__;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGoKartMovementReplicator* MovementReplicator;

	//UPROPERTY(VisibleAnywhere)
	//	class UBoxComponent* TriggerVolume;

	

	void GetOutTheCar();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendGetOutTheCar();
	class AController* AIController;
private:
	UPROPERTY(VisibleAnywhere, replicated)
	APawn* Rider;


};
