// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshTop;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshBarrelL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshBarrelR;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void FindBestTarget();
	UFUNCTION(BlueprintCallable)
		bool HasLineOfSight(FVector From, FVector To, TArray<AActor*> ActorsToIgnore);
	UFUNCTION(BlueprintCallable)
		void UpdateFire();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Fire();
	UFUNCTION( BlueprintCallable)
		void UpdateTurretRotation(float DeltaSeconds);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_DestroyEffect();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_Fire();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SensingRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeOfLastFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RateOfFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotateSpeed;
	UPROPERTY(EditAnywhere)
		int TeamNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UParticleSystem* DestroyImpactParticles;
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;
	class UTurretStatComponent* TurretStat;//�ణ �ֵ�



};
