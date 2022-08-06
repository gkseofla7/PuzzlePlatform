// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PuzzlePlatforms.h"
#include "Components/ActorComponent.h"

#include "MyPlayerData.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

	void SetHP(float NewHP);
	float GetHP() { return CurrentHP; }
	float GetHPRatio();

	FOnHPChangedDelegate OnHPChanged;

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
	UPROPERTY(ReplicatedUsing = OnRep_HP, Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	UFUNCTION()
	void OnRep_HP();


	//�𸮾� ������Ʈ���� ����ȭ ����� �־ ������Ʈ�� UPROPERTPY �Ӽ��� �����ϰ� �ε��� �� ����
};
