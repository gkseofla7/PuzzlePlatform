// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"
#include "../MyPlayerState.h"
#include "../UI/MapWidget.h"
#include "../AbilitySystem/UI/CastBarWidget.h"
#include "../AbilitySystem/UI/Spellbook_UI.h"
#include "../AbilitySystem//UI/ActionBarWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

bool UPlayerInfoWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	HpBar->SetPercent(1.0);
	MpBar->SetPercent(1.0);

	return true;
}

void UPlayerInfoWidget::BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	//CurrentCharacterStat->SetHP(80);
	//Widget ÃÊ±âÈ­
	PlayerName->SetText(FText::FromString(TEXT("Not Yet")));
	HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	MpBar->SetPercent(CurrentCharacterStat->GetMPRatio());
	auto hp = FString::Printf(TEXT("%d"), int(CurrentCharacterStat->CurrentHP));
	auto	hpText = FText::FromString(hp);
	HpNum->SetText(hpText);

	auto mp = FString::Printf(TEXT("%d"), (int)CurrentCharacterStat->CurrentMP);
	auto	mpText = FText::FromString(mp);
	MpNum->SetText(mpText);
	ExpBar->SetPercent(CurrentCharacterStat->GetExpRatio());
	SetLevel();

	NewCharacterStat->OnMPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			MpBar->SetPercent(CurrentCharacterStat->GetMPRatio());
			auto mp = FString::Printf(TEXT("%d"), (int)CurrentCharacterStat->CurrentMP);
		
			auto	mpText = FText::FromString(mp);
			MpNum->SetText(mpText);
		}
		});

	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());
			auto hp = FString::Printf(TEXT("%d"), (int)CurrentCharacterStat->CurrentHP);

			auto	hpText = FText::FromString(hp);
			HpNum->SetText(hpText);
		}
		});

	NewCharacterStat->OnExpChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("ExpRatio %f"), CurrentCharacterStat->GetExpRatio());
			UE_LOG(LogTemp, Warning, TEXT("CurrentExp %d"), CurrentCharacterStat->CurrentExp);
			UE_LOG(LogTemp, Warning, TEXT("NextExp %d"), CurrentCharacterStat->CurrentStatData->NextExp);
			ExpBar->SetPercent(CurrentCharacterStat->GetExpRatio());
		}
		});

	NewCharacterStat->OnLevelChanged.AddUObject(this, &UPlayerInfoWidget::SetLevel);

	ActionBar_UI->BindCharacterStat(NewCharacterStat);
	SetLevel();


}

void UPlayerInfoWidget::BindCharacterName(FText NewName)
{
	PlayerName->SetText(NewName);
}

void UPlayerInfoWidget::SetLevel()
{
	int NewLevel = CurrentCharacterStat->Level;
	FString S_Level = FString::Printf(TEXT("%d"), NewLevel);
	T_Level->SetText(FText::FromString(S_Level));
}

void UPlayerInfoWidget::ToggleMap()
{
	if (MapWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		MapWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MapWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerInfoWidget::ToggleSpellBook()
{
	if (Spellbook_UI->GetVisibility() == ESlateVisibility::Hidden)
	{
		Spellbook_UI->SetVisibility(ESlateVisibility::Visible);

	}
	else
	{
		Spellbook_UI->SetVisibility(ESlateVisibility::Hidden);
	}
}

