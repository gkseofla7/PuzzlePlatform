// Fill out your copyright notice in the Description page of Project Settings.


#include "SetNameMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "../PlayerInfoWidget.h"
#include "../MyPlayerController.h"
bool USetNameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(ConfirmButton != nullptr)) return false;
	ConfirmButton->OnClicked.AddDynamic(this, &USetNameMenu::ConfirmID);


	return true;
}



void USetNameMenu::ConfirmID()
{


	//FText SendPlayerID = PlayerID->GetText();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *SendPlayerID.ToString());
	if (!(PlayerID->GetText().EqualTo( FText::GetEmpty())))
	{
		Cast<AMyPlayerController>(GetOwningPlayer())->HUDWidget->BindCharacterName(PlayerID->GetText());

		//PlayerController->SetName(SendPlayerID.ToString());//애초에 여기까지
		Teardown();
		//이제 이 메뉴 없애고 
	}
}