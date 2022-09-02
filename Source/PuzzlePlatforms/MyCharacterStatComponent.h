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
	void DecreaseMP(float NewMP) { SetMP(CurrentMP - NewMP); }
	float GetHP() { return CurrentHP; }
	float GetMP() { return CurrentMP; }
	float GetHPRatio();
	float GetMPRatio();

	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;//Post Initialize ���� �Ͼ

public:
	void SetNewLevel(int32 NewLevel);

private:
	FMyCharacterrData* CurrentStatData = nullptr;

	//private�̿��� blueprint���� ���ٰ���
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HP, Transient,   Category = Stat)
		float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentMP;

	UFUNCTION()
	void OnRep_HP();


	//�𸮾� ������Ʈ���� ����ȭ ����� �־ ������Ʈ�� UPROPERTPY �Ӽ��� �����ϰ� �ε��� �� ����
};
