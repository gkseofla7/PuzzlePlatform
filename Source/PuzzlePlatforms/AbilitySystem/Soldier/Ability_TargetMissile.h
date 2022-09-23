// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Projectile.h"
#include "Ability_TargetMissile.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_TargetMissile : public AAbility_Projectile
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	AAbility_TargetMissile();
	void ActivateEffect_Implementation() override;
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetVisibility();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetVisibility();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* MissileComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UTargetMissileMovementComponent* MissileMovementComponent;
	class ASoldier* SoldierRef;
	class AActor* TargetPlayer;
};
