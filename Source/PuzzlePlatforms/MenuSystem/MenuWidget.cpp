// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInteface)
{
	UE_LOG(LogTemp, Warning, TEXT("Set Interface!"));
	this->MenuInterface = MenuInteface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FInputModeUIOnly UIOnly;

	this->bIsFocusable = true;
	//UIOnly.SetWidgetToFocus(Menu->TakeWidget());
	UIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(UIOnly);

	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = false;
	this->bIsFocusable = false;

	FInputModeGameOnly GameOnly;

	PlayerController->SetInputMode(GameOnly);



}

