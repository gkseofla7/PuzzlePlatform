// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindTurretStat(class UTurretStatComponent* NewTurretStat);
	void BindMonsterStat(class UMonsterStatComponent* NewMonsterStat);
protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();


private:
	TWeakObjectPtr<class UTurretStatComponent> CurrentTurretStat;
	TWeakObjectPtr<class UMonsterStatComponent> CurrentMonsterStat;
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
