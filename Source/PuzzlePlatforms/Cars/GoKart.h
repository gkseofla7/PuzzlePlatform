// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGoKartMovementComponent* OurMovementComponent_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGoKartMovementReplicator* MovementReplicator;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* TriggerVolume;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	bool Ridden = false;

	void GetOutTheCar();



private:
	APawn* Rider;
};
