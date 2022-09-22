// Fill out your copyright notice in the Description page of Project Settings.


#include "SetNameMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../MyPlayerState.h"

#include "../UI/PlayerInfoWidget.h"
#include "../MyPlayerController.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "GameFramework/PlayerState.h"
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
		//Cast<AMyPlayerController>(GetOwningPlayer())->HUDWidget->BindCharacterName(PlayerID->GetText());
		//Cast< UPuzzlePlatformsGameInstance>(GetGameInstance())->PlayerName = PlayerID->GetText();
		auto MyPlayerState = Cast< AMyPlayerState>(GetOwningPlayerState());
		MyPlayerState->Server_SetPlayerName(PlayerID->GetText());//�ڱ��ڽŸ��� �ȹٲ� �� ������ �ҷ��ߵ�
		//PlayerController->SetName(SendPlayerID.ToString());//���ʿ� �������
		Teardown();
		//���� �� �޴� ���ְ� 
	}
}