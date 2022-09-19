// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarWidget.h"

#include "../Ability.h"
#include "ActionBarSlotWidget.h"
#include "../../PlayersComponent/MyCharacterStatComponent.h"
void UActionBarWidget::BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat)//������Ѿߵ� �� ����������
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
					//��ų�� ������ �� ������,,
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

