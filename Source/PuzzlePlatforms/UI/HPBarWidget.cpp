// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"
#include "../Turret/TurretStatComponent.h"
#include "../AI/MonsterStatComponent.h"

#include "Components/ProgressBar.h"


void UHPBarWidget::BindTurretStat(UTurretStatComponent* NewTurretStat)
{
	ABCHECK(nullptr != NewTurretStat);

	CurrentTurretStat = NewTurretStat;
	CurrentTurretStat->OnHPChanged.AddUObject(this, &UHPBarWidget::UpdateHPWidget);
}

void UHPBarWidget::BindMonsterStat(UMonsterStatComponent* NewMonsterStat)
{
	ABCHECK(nullptr != NewMonsterStat);

	CurrentMonsterStat = NewMonsterStat;
	NewMonsterStat->OnHPChanged.AddUObject(this, &UHPBarWidget::UpdateHPWidget);
}


void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateHPWidget();
}

void UHPBarWidget::UpdateHPWidget()
{
	if (CurrentMonsterStat.IsValid())
	{
		if (nullptr != PB_HPBar)
		{
			PB_HPBar->SetPercent(CurrentMonsterStat->GetHPRatio());
		}
	}
	else if (CurrentTurretStat.IsValid())
	{
		if (nullptr != PB_HPBar)
		{
			PB_HPBar->SetPercent(CurrentTurretStat->GetHPRatio());
		}
	}
}