// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"
#include "PointOfInterestWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"

#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
UMinimapWidget::UMinimapWidget(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPointOfInterestWidget> PointOfInterestWidgetBPClass(TEXT("/Game/MiniMap/BP_PointOfInterestWidget"));
	if (PointOfInterestWidgetBPClass.Succeeded())
	{
		PointOfInterestWidgetClass = PointOfInterestWidgetBPClass.Class;
	}
}
void UMinimapWidget::AddsPOI(AActor* Owner)
{
	
	auto FindComp = Owner->GetComponentByClass(UPointOfInterestComponent::StaticClass());
	ABCHECK(FindComp != nullptr);
	auto PointOfInterestComponent = Cast< UPointOfInterestComponent>(FindComp);
	UPointOfInterestWidget* PointOfInterestWidget = CreateWidget<UPointOfInterestWidget>(GetWorld(), PointOfInterestWidgetClass);
	ABCHECK(PointOfInterestWidget!= nullptr);
	PointOfInterestWidget->CustomInitialize(Owner, PointOfInterestComponent->IsStatic);
	auto MySlot = MapOverlay->AddChildToOverlay(PointOfInterestWidget);
	MySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	MySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	
}