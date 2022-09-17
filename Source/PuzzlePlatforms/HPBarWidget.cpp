// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"
#include "PlayersComponent/MyCharacterStatComponent.h"
#include "AI/MonsterStatComponent.h"

#include "Components/ProgressBar.h"


void UHPBarWidget::BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UHPBarWidget::UpdateHPWidget);
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
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{

			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
	else if (CurrentMonsterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentMonsterStat->GetHPRatio());
		}
	}
}