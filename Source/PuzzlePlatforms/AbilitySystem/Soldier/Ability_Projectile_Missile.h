// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Projectile.h"
#include "Ability_Projectile_Missile.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile_Missile : public AAbility_Projectile
{
	GENERATED_BODY()

public:
	AAbility_Projectile_Missile();
	virtual void BeginPlay();
	virtual void ActivateEffect_Implementation() override;
	virtual void SetAbilityLevel() override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DelayCollision();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetVisibility( );
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetVisibility( );
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Spark(FVector Location);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* MissileComponent; 
	UPROPERTY()
	class ASoldier* SoldierRef;
	UPROPERTY()
	class UParticleSystem* ParticleTemplate;
};
