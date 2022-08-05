// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "MyCharacterStatComponent.h"

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
	CurrentCharacterStat->SetHP(80);
	HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio())
			UE_LOG(LogTemp, Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->CurrentHP)
		}
		});

	NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat.IsValid())
		{
			HpBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
		});

}