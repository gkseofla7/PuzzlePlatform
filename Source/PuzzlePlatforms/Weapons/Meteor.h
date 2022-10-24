// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_Spark(FVector Location);
	void ActiveDecal();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UDecalComponent* DecalComponent;
	float DamageAmount = 0.f;
	UPROPERTY()
	class UParticleSystem* ParticleTemplate;;


};
