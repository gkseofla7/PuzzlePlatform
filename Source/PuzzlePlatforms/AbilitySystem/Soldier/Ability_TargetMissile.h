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
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetVisibility();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetVisibility();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Spark(FVector Location);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetActive();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetActive();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetTarget(AActor* NewTarget);

	virtual void SetAbilityLevel() override;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* MissileComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UTargetMissileMovementComponent* MissileMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UTargetMissileReplicateComponent* MissileReplicateComponent;
	class ASoldier* SoldierRef;
	class AActor* TargetPlayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USceneComponent* MeshOffsetRoot;
	class UParticleSystem* ParticleTemplate;

	bool bActive = false;
};
