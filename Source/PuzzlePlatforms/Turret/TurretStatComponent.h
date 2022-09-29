// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UTurretStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurretStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	float GetHPRatio()
	{
		return CurrentHp / MaxHp;
	}
	void SetHp(float NewHp);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHp(float NewHp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetHp(float NewHp);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FOnHPChangedDelegate OnHPChanged;

	float MaxHp = 1000.f;
	float CurrentHp = 1000.f;
		
};
