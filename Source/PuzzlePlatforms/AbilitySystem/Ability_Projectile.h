// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Ability_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile : public AAbility
{
	GENERATED_BODY()
protected:
	AAbility_Projectile();
	virtual void BeginPlay() override;
public:
	
	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetVelocity(FVector NewVelocity);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetVelocity(FVector NewVelocity);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AddLocation(FVector RelativeLocation);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_AddLocation(FVector RelativeLocation);
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class	UProjectileMovementComponent* ProjectileMovement_;

};
