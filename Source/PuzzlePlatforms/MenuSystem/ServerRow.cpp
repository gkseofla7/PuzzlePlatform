// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"



void UServerRow::Setup(UMainMenu* _Parent, uint32 _Index)
{
	if (!ensure(RowButton != nullptr)) return;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	Parent = _Parent;
	Index = _Index;
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}

