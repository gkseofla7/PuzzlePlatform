// Fill out your copyright notice in the Description page of Project Settings.


#include "MapWidget.h"
#include "PointOfInterestWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"
#include "../Character_Master.h"

#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPointOfInterestWidget> PointOfInterestWidgetBPClass(TEXT("/Game/MiniMap/BP_PointOfInterestWidget"));
	if (PointOfInterestWidgetBPClass.Succeeded())
	{
		PointOfInterestWidgetClass = PointOfInterestWidgetBPClass.Class;
	}
}
void UMapWidget::AddsPOI(AActor* Owner)
{
	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto MyPawn = Controller->GetPawn();
	
	if (MyPawn != nullptr)
	{
		auto PuzzleCharacter = Cast < ACharacter_Master>(Owner);
		if (PuzzleCharacter != nullptr)
		{

			if (PuzzleCharacter->TeamNum != Cast<ACharacter_Master>(MyPawn)->TeamNum)
			{//다른team이면 맵에 안그림
				return;
			}
		}
	}

	auto FindComp = Owner->GetComponentByClass(UPointOfInterestComponent::StaticClass());
	ABCHECK(FindComp != nullptr);
	auto PointOfInterestComponent = Cast< UPointOfInterestComponent>(FindComp);
	UPointOfInterestWidget* PointOfInterestWidget = CreateWidget<UPointOfInterestWidget>(GetWorld(), PointOfInterestWidgetClass);
	ABCHECK(PointOfInterestWidget != nullptr);
	PointOfInterestWidget->CustomInitialize(Owner, PointOfInterestComponent->IsStatic, false);
	auto MySlot = BigMapOverlay->AddChildToOverlay(PointOfInterestWidget);
	MySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	MySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

}