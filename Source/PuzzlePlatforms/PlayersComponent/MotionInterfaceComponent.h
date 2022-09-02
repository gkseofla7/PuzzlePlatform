// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionInterfaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UMotionInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMotionInterfaceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Server_SendRide(AActor* _Car, APawn* _Rider);

	virtual void Server_SendAttack();

	virtual void DisableActor(bool toHide);
	void AbilitySpawn(TSubclassOf<class AAbility>AbilityClass);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill1Clicked(TSubclassOf<AAbility>AbilityClass);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill2Clicked(TSubclassOf<AAbility>AbilityClass);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill3Clicked(TSubclassOf<AAbility>AbilityClass);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill4Clicked(TSubclassOf<AAbility>AbilityClass);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Skill5Clicked(TSubclassOf<AAbility>AbilityClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetTargetPlayer(class APuzzlePlatformsCharacter* NewTarget);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetTargetPlayer(class APuzzlePlatformsCharacter* NewTarget);
};
