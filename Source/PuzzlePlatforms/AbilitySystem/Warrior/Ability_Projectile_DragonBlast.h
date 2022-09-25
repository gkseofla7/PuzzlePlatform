// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability_Projectile.h"
#include "Ability_Projectile_DragonBlast.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile_DragonBlast : public AAbility_Projectile
{
	GENERATED_BODY()
public:
	AAbility_Projectile_DragonBlast();
protected:
	void BeginPlay() override;
	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetAbilityLevel() override;
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PlayNextAnimation();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_PlayNextAnimation();
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;
	bool Toggle = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> SweepArray;
	TSubclassOf<class AAbility>  Ability_Buff_Fortitude_Class;
};
