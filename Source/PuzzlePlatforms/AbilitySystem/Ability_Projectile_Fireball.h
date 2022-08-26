// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Projectile.h"
#include "Ability_Projectile_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile_Fireball : public AAbility_Projectile
{
	GENERATED_BODY()
protected:
	AAbility_Projectile_Fireball();
	virtual void BeginPlay() override;

	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
public:
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetVelocity(FVector NewVelocity);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetVelocity(FVector NewVelocity);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;
};
