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
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));//이런방법이 있구만~
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UHPBarWidget::UpdateHPWidget()
{
	if (CurrentMonsterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentMonsterStat->GetHPRatio());
		}
	}
	else if (CurrentTurretStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentTurretStat->GetHPRatio());
		}
	}
}