// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop.h"

void UDragDrop::CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass)
{
	AbilityClass = NewAbilityClass;

}