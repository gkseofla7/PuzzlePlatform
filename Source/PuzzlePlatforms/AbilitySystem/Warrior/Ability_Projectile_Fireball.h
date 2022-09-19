// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Projectile.h"
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

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Spark(FVector Location);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_DeactivateParticle();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PlayNextAnimation();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_PlayNextAnimation();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;

	class UParticleSystem* ParticleTemplate;

};
