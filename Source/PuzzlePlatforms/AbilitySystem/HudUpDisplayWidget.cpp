// Fill out your copyright notice in the Description page of Project Settings.


#include "HudUpDisplayWidget.h"
#include "CastBarWidget.h"
UHudUpDisplayWidget::UHudUpDisplayWidget(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	//ConstructorHelpers::FClassFinder<UUserWidget> CastBarBPClass(TEXT("/Game/AbilitySystem/UI/ActionBarUI/CastBar_UI"));
	//if (!ensure(CastBarBPClass.Class != nullptr)) return;
	//CastBarClass = CastBarBPClass.Class;
}

//UUserWidget* UHudUpDisplayWidget::DisplayCastBar(class AAbility* CastToAbility)
//{
//	CastBar_UI->SetVisibility(ESlateVisibility::Visible);
//	CastBar_UI->Abil
//}