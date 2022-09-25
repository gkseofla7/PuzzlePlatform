// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarSlotWidget.h"


#include "../Ability.h"
#include"../../Character_Master.h"
#include "DragDrop.h"
#include "ActionBarWidget.h"


#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
UActionBarSlotWidget::UActionBarSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void UActionBarSlotWidget::NativePreConstruct()
{

	Super::NativePreConstruct();
	IconInitialize();

	UpdateAppearance();
}

void UActionBarSlotWidget::IconInitialize()
{
	if (AbilityClass != nullptr)
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
		AAbility* DefaultActor = Cast<AAbility>(AbilityClass->GetDefaultObject(true));
		IconImage->SetBrushFromTexture(DefaultActor->AbilityDetails.Icon);

	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);

	}
}


void UActionBarSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (CooldownBar->IsVisible() == true)
	{
		float RemaingTime = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), CooldownTimerHandler);
		float ElapsedTime = UKismetSystemLibrary::K2_GetTimerElapsedTimeHandle(GetWorld(), CooldownTimerHandler);

		float Total = RemaingTime + ElapsedTime;
		CooldownBar->SetPercent(UKismetMathLibrary::NormalizeToRange(RemaingTime, 0, Total));
		
	}
	if (IsManaAvailable == false)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		CooldownBar->SetPercent(1);
	}
}

void UActionBarSlotWidget::StartCooldown()
{
	IsAvailable = false;
	UpdateAppearance();

	auto ability = AbilityClass.GetDefaultObject();
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandler, this, &UActionBarSlotWidget::EndCooldown, ability->AbilityDetails.CoolDown, false);

}

void UActionBarSlotWidget::EndCooldown()
{
	IsAvailable = true;
	UpdateAppearance();
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), CooldownTimerHandler);
}

void UActionBarSlotWidget::UpdateAppearance()
{
	if (IsAvailable == false)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		CooldownBar->SetPercent(1);
	}
	else
	{
		CooldownBar->SetVisibility(ESlateVisibility::Hidden);
		CooldownBar->SetPercent(0);
	}

}

bool UActionBarSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto DragDrop = Cast<UDragDrop>(InOperation);
	if (DragDrop == nullptr)
		return false;
	if (IsAvailable == false)//쿨타임중일때는 변경 못하게
		return false;
	if (AbilityClass == DragDrop->AbilityClass)//같은 클래스면 변경X
		return false;
	AbilityClass = DragDrop->AbilityClass;
	for (auto ActionBarSlot : ParentsWidget->ActionBarSlotArray)
	{
		if (ActionBarSlot->AbilityClass == nullptr)
			continue;
		if (ActionBarSlot == this)//나 자신이면 패스
			continue;
		if (ActionBarSlot->AbilityClass == AbilityClass)//내꺼랑 같으면
		{
			if (ActionBarSlot->IsAvailable == false)
			{
				AbilityClass = nullptr;
				return false;
			}
			ActionBarSlot->AbilityClass = nullptr;
			ActionBarSlot->IconInitialize();
		}
	}



	IconImage->SetVisibility(ESlateVisibility::Visible);
	AAbility* ability = Cast<AAbility>(AbilityClass->GetDefaultObject());
	IconImage->SetBrushFromTexture(ability->AbilityDetails.Icon);
	IsAvailable = true;
	if(ParentsWidget->CurrentCharacterStat!=nullptr)
		ParentsWidget->BindCharacterStat(ParentsWidget->CurrentCharacterStat);

	//ParentsWidget; 여기서는 이미 스킬이 들어가 있으면 거기꺼 제거

	return true;
}