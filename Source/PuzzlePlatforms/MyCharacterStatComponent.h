// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PuzzlePlatforms.h"
#include "Components/ActorComponent.h"

#include "MyPlayerData.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

	void SetHP(float NewHP);
	void SetMP(float NewMP);
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

	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;//Post Initialize 전에 일어남

public:
	void SetNewLevel(int32 NewLevel);

private:
	FMyCharacterrData* CurrentStatData = nullptr;

	//private이여도 blueprint에서 접근가능
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HP, Transient,   Category = Stat)
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP;

	UFUNCTION()
	void OnRep_HP();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHP(float NewHp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetHP(float NewHp);

	//언리얼 오브젝트에는 직렬화 기능이 있어서 오브젝트의 UPROPERTPY 속성을 저장하고 로딩할 수 있음
};
