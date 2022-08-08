// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"


#include "MotionReplicatorInterface.generated.h"

// This class does not need to be modified.UINTERFACE(MinimalAPI)
UINTERFACE( MinimalAPI, meta = (BlueprintSpawnableComponent))
class UMotionReplicatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEPLATFORMS_API IMotionReplicatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Server_SendRide(AActor* _Car, APawn* _Rider) = 0;

	virtual void Server_SendAttack()=0;


};
