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
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPlayerAnimInstance* AsPlayerAnimInstance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class	UProjectileMovementComponent* ProjectileMovement_;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UParticleSystemComponent* ParticleSystemComponent;
};
