// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarWidget.h"

#include "../Ability.h"
#include "ActionBarSlotWidget.h"
#include "../../PlayersComponent/MyCharacterStatComponent.h"
void UActionBarWidget::BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat)//실행시켜야됨 각 넣을때마다
{
	CurrentCharacterStat = NewCharacterStat;
	//CurrentCharacterStat->SetHP(80);

	NewCharacterStat->OnMPChanged.AddLambda([this]() -> void {
		if (CurrentCharacterStat!=nullptr)
		{

			for (auto ActionBarSlot : ActionBarSlotArray)
			{
				if (ActionBarSlot->AbilityClass == nullptr)
					continue;
				if (CurrentCharacterStat->CurrentMP < ActionBarSlot->AbilityClass.GetDefaultObject()->AbilityDetails.Cost)
				{
					//스킬의 마나가 더 높으면,,
					ActionBarSlot->IsManaAvailable = false;
					ActionBarSlot->UpdateAppearance();
				}
				else
				{
					ActionBarSlot->IsManaAvailable = true;
					ActionBarSlot->UpdateAppearance();

				}

			}
		}
		});


}

