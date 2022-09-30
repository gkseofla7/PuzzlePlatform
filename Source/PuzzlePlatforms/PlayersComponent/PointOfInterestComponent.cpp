// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestComponent.h"
#include "../UI/MinimapWidget.h"
#include "../MyPlayerController.h"
#include "../UI/PlayerInfoWidget.h"
#include "../UI/MinimapWidget.h"
#include "../UI/MapWidget.h"
// Sets default values for this component's properties
UPointOfInterestComponent::UPointOfInterestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPointOfInterestComponent::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle PossessCharacterHandle;
	FTimerDelegate PossessCharacterDelegate = FTimerDelegate::CreateUObject(this, &UPointOfInterestComponent::AddPOI);
	GetOwner()->GetWorldTimerManager().SetTimer(PossessCharacterHandle, PossessCharacterDelegate, 2.5f, false);


}


// Called every frame
void UPointOfInterestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPointOfInterestComponent::AddPOI()
{
	SetIcon();
	auto Pawn = Cast<APawn>(GetOwner());
	if (Pawn != nullptr && Pawn->IsLocallyControlled() == true&&Pawn->IsPlayerControlled() )//자기 자신은 안그림
		return;
	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABCHECK(Controller!=nullptr);
	auto MyController = Cast<AMyPlayerController>(Controller);
	ABCHECK(MyController!=nullptr);
	
	auto PlayerInfoHUDWidget = MyController->PlayerInfoHUDWidget;
	ABCHECK(	PlayerInfoHUDWidget!=nullptr);
	auto Minimap_Widget = PlayerInfoHUDWidget->Minimap_Widget;
	if(Minimap_Widget != nullptr)
		Minimap_Widget->AddsPOI(GetOwner());
	auto MapWidget = PlayerInfoHUDWidget->MapWidget;
	if(MapWidget!=nullptr)
		MapWidget->AddsPOI(GetOwner());
}