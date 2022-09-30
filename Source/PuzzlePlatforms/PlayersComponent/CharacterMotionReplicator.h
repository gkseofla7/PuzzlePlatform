// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMotionReplicator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UCharacterMotionReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterMotionReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void Server_SendRide(AActor* _Car, APawn* _Rider);
	virtual void Server_SendAttack();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_BindCharacterStatToWidget();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void  NetMulticast_BindCharacterStatToWidget();
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
		void Server_SetMaxWalkSpeed(float NewWalkSpeed);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void  NetMulticast_SetMaxWalkSpeed(float NewWalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetTargetPlayer(class ACharacter_Master* NewTarget);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetTargetPlayer(class ACharacter_Master* NewTarget);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsAttacking(bool NewIsAttacking);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetIsAttacking(bool NewIsAttacking);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetUsingSkill(bool NewUsingSkill);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetUsingSkill(bool NewUsingSkill);

public:
	class ACharacter_Master* PlayerRef;
};
