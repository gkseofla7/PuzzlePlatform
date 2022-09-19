// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellbookSlot_UI.h"
#include "../Ability.h"

#include "Components/Image.h"

void USpellbookSlot_UI::CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass)
{
	AbilityClass = NewAbilityClass;

	auto ability = AbilityClass.GetDefaultObject();
	SpellImage->SetBrushFromTexture(ability->AbilityDetails.Icon);
}