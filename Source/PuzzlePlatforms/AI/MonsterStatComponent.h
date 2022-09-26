// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../DataTable/MonstersData.h"
#include "EnumMonsterType.h"
#include "MonsterStatComponent.generated.h"




DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API UMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;//Post Initialize 전에 일어남
public:
	// Sets default values for this component's properties
	UMonsterStatComponent();
	void CustomInitializeComponent(EMonsterEnum NewMonsterEnum);
	void SetHP(float NewHP);
	void LevelUp(int32 NewLevel);
	void IncreaseHP(float NewHP) {
		if (CurrentHP == CurrentStatData->MaxHP && NewHP > 0)
			return;
		SetHP(CurrentHP + NewHP);
	}
	float GetHP() { return CurrentHP; }

	float GetHPRatio();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetHP(float NewHp);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetHP(float NewHp);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = Stat)
		float CurrentHP;
	int DropExp;

	float AttackDamage;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	FMonsterData* CurrentStatData = nullptr;
	EMonsterEnum MonsterEnum;
	FOnHPChangedDelegate OnHPChanged;
	//FOnMPChangedDelegate OnMPChanged;
};
