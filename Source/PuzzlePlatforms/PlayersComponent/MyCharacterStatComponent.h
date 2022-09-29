// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Components/ActorComponent.h"

#include "../DataTable/MyPlayerData.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNameChangedDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;//Post Initialize 전에 일어남
public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

	void SetHP(float NewHP);
	void SetMP(float NewMP);
	void SetExp(int NewExp);
	void SetLevel(float NewLevel);
	//void SetName(FText NewName);

	void IncreaseHP(float NewHP) {
		if (CurrentStatData == nullptr)
			return;
		if (CurrentHP == CurrentStatData->MaxHP&&NewHP > 0)
			return;
		SetHP(CurrentHP + NewHP); }
	void IncreaseMP(float NewMP) {
		if (CurrentStatData == nullptr)
			return;
		if (CurrentMP == CurrentStatData->MaxMP && NewMP > 0)
			return;
		SetMP(CurrentMP + NewMP); }
	float GetHP() { return CurrentHP; }
	float GetMP() { return CurrentMP; }
	float GetHPRatio();
	float GetMPRatio();
	float GetExpRatio();
	
	//UFUNCTION()
	//	void OnRep_HP();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHP(float NewHp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetHP(float NewHp);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetMP(float NewMp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetMP(float NewMp);
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetLevel(float NewLevel);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetLevel(float NewLevel);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetExp(int NewExp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetExp(int NewExp);

	void Respawn();


public:
	class ACharacter_Master* CharacterRef;
	//ReplicatedUsing = OnRep_HP,
	UPROPERTY(EditAnywhere, Transient)
		float CurrentHP;
	UPROPERTY(EditAnywhere, Transient)
		float CurrentMP;
	int32 Level;
	int AttackDamage;
	FText Name;
	int CurrentExp = 0;
	FMyCharacterrData* CurrentStatData = nullptr;
	FOnHPChangedDelegate OnHPChanged;//걍 이거 다 리셋하는게..?
	FOnMPChangedDelegate OnMPChanged;
	FOnExpChangedDelegate OnExpChanged;
	FOnLevelChangedDelegate OnLevelChanged;
	FOnNameChangedDelegate OnNameChanged;

};
