// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPBarWidget.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHPBarWidget::BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UPlayerHPBarWidget::UpdateHPBar);
	NewCharacterStat->OnMPChanged.AddUObject(this, &UPlayerHPBarWidget::UpdateMPBar);
	NewCharacterStat->OnLevelChanged.AddUObject(this, &UPlayerHPBarWidget::UpdateLevelText);
	NewCharacterStat->OnNameChanged.AddUObject(this, &UPlayerHPBarWidget::UpdateNameText);
	UpdateHPBar();
	UpdateMPBar();
	UpdateLevelText();
	UpdateNameText();
}



void UPlayerHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPlayerHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	/*if(CurrentCharacterStat!=nullptr)
		UE_LOG(LogTemp, Warning, TEXT("HP : %f"), CurrentCharacterStat->GetHPRatio());*/
}

void UPlayerHPBarWidget::UpdateHPBar()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != PB_HPBar)
		{

			if (CurrentCharacterStat->GetOwner()->HasAuthority())
			{
				UE_LOG(LogTemp, Warning, TEXT("Server %s HP : %f"), *(CurrentCharacterStat->GetOwner()->GetName()), CurrentCharacterStat->GetHPRatio());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client %s HP : %f"), *(CurrentCharacterStat->GetOwner()->GetName()), CurrentCharacterStat->GetHPRatio());
			}
			PB_HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());

		}
	}

}

void UPlayerHPBarWidget::UpdateMPBar()
{
	if (CurrentCharacterStat.IsValid())
	{

		if (nullptr != PB_MPBar)
		{

			PB_MPBar->SetPercent(CurrentCharacterStat->GetMPRatio());
		}
	}
}

void UPlayerHPBarWidget::UpdateLevelText()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != T_Level)
		{
			FString Level = FString::Printf(TEXT("% d"), CurrentCharacterStat->Level);
			T_Level->SetText(FText::FromString(Level));
		}
	}
}


void UPlayerHPBarWidget::UpdateNameText()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != T_Name)
		{
			T_Name->SetText(CurrentCharacterStat->Name);
			
		}
	}
}
