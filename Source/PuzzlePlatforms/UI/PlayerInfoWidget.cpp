// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"
#include "../MyPlayerState.h"


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
	
	PlayerName->SetText(FText::FromString(TEXT("Not Yet")));
	HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());

	auto hp = FString::Printf(TEXT("%d"), int(CurrentCharacterStat->CurrentHP));
	auto	hpText = FText::FromString(hp);
	HpNum->SetText(hpText);

	auto mp = FString::Printf(TEXT("%d"), (int)CurrentCharacterStat->CurrentMP);
	auto	mpText = FText::FromString(mp);
	MpNum->SetText(mpText);

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

}

void UPlayerInfoWidget::BindCharacterName(FText NewName)
{
	PlayerName->SetText(NewName);
}