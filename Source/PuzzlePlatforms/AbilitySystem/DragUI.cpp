// Fill out your copyright notice in the Description page of Project Settings.


#include "DragUI.h"

#include "Ability.h"

#include "Components/Image.h"

void UDragUI::CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass)
{
	AbilityClass = NewAbilityClass;
	auto ability = AbilityClass.GetDefaultObject();
	DragIcon->SetBrushFromTexture(ability->AbilityDetails.Icon);
}