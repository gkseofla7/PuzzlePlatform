// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionReplicatorInterface.h"
#include "SoldierMotionReplicator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API USoldierMotionReplicator : public UActorComponent, public IMotionReplicatorInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoldierMotionReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendRide(AActor* _Car, APawn* _Rider) override;
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendAttack() override;

	UFUNCTION(BlueprintCallable, Category = "Disable")
		void DisableActor(bool toHide);


private:
	UPROPERTY()
		class USoldierAnimInstance* MyAnim;
};
