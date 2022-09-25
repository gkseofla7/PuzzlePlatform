// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"
#include "PointOfInterestWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"
#include "../Character_Master.h"

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
	auto MyCharacter = Cast<ACharacter_Master>(Owner);
	if (MyCharacter != nullptr)
	{
		auto MyPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
		if (MyPawn != nullptr)
		{
			if (MyCharacter->TeamNum == Cast< ACharacter_Master>(MyPawn)->TeamNum)//같은팀이면
			{
				PointOfInterestComponent->IsStatic = true;//이러면 멀리 떨어져도 보임
			}
		}
	}
	PointOfInterestWidget->CustomInitialize(Owner, PointOfInterestComponent->IsStatic, true);
	auto MySlot = MapOverlay->AddChildToOverlay(PointOfInterestWidget);
	MySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	MySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	
}