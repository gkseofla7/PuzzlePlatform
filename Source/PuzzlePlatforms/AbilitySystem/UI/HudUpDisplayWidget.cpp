// Fill out your copyright notice in the Description page of Project Settings.


#include "HudUpDisplayWidget.h"
#include "CastBarWidget.h"
#include "Spellbook_UI.h"
UHudUpDisplayWidget::UHudUpDisplayWidget(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{

}

void UHudUpDisplayWidget::ToggleSpellBook()
{
	if(Spellbook_UI->GetVisibility() ==ESlateVisibility::Hidden)
	{
		Spellbook_UI->SetVisibility(ESlateVisibility::Visible);
		
	}
	else
	{
		Spellbook_UI->SetVisibility(ESlateVisibility::Hidden);
	}
}