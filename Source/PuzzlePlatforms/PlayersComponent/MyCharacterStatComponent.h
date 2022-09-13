// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Components/ActorComponent.h"

#include "../DataTable/MyPlayerData.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
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
	void SetNewLevel(int32 NewLevel);
	void IncreaseHP(float NewHP) {
		if (CurrentHP == CurrentStatData->MaxHP&&NewHP > 0)
			return;
		SetHP(CurrentHP + NewHP); }
	void IncreaseMP(float NewMP) {
		if (CurrentMP == CurrentStatData->MaxMP && NewMP > 0)
			return;
		SetMP(CurrentMP + NewMP); }
	float GetHP() { return CurrentHP; }
	float GetMP() { return CurrentMP; }
	float GetHPRatio();
	float GetMPRatio();
	//UFUNCTION()
	//	void OnRep_HP();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHP(float NewHp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetHP(float NewHp);



public:
	//ReplicatedUsing = OnRep_HP,
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Transient,   Category = Stat)
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	FMyCharacterrData* CurrentStatData = nullptr;

	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;

};
