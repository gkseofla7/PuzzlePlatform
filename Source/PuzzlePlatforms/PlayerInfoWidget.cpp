// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "MyCharacterStatComponent.h"
#include "MyPlayerState.h"


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
	NewCharacterStat->OnMPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			MpBar->SetPercent(CurrentCharacterStat->GetMPRatio());
		}
		});

	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
		});

}

void UPlayerInfoWidget::BindCharacterName(FText NewName)
{
	PlayerName->SetText(NewName);
}