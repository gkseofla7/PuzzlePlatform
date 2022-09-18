// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PlayerInfoWidget.h"
#include "PuzzlePlatformsCharacter.h"
#include "PuzzlePlatformsGameInstance.h"
#include "AbilitySystem/HudUpDisplayWidget.h"
#include"PlayersComponent/MyCharacterStatComponent.h"

#include "MyLobbyGameMode.h"
#include "Net/UnrealNetwork.h"
AMyPlayerController::AMyPlayerController()
{
	
	//static ConstructorHelpers::FClassFinder< UUserWidget> NEWUI_HUD_C(TEXT("/Game/AbilitySystem/UI/HeadsUpDisplay"));
	///Game/AbilitySystem/UI/HeadsUpDisplay


	bReplicates = true;
	Level = 1;
	//CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	//if (NEWUI_HUD_C.Succeeded())
	//{
	//	NewHUDWidgetClass = NEWUI_HUD_C.Class;
	//}
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}
void AMyPlayerController::OnPossess(APawn* aPawn)
{

	Super::OnPossess(aPawn);
	
	//auto MyCharacter = Cast<APuzzlePlatformsCharacter>(aPawn);
	//if (MyCharacter != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s : POSSEEEEEESS %s %d"), *GetName(), *MyCharacter->GetName(), Level);
	//	MyCharacter->Multicast_SetLevel(Level);

	//}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	

	if (IsLocalController() )
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		//위젯은 무조건 beginplay에서 초기화시켜야됨

		//NewHUDWidget = CreateWidget<UUserWidget>(this, NewHUDWidgetClass);


	}

}



//UPlayerInfoWidget* AMyPlayerController::GetHudWidget() const
//{
//	return HUDWidget;
//}

void AMyPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture (true);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void AMyPlayerController::SetInputModeGame()
{
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);

	SetInputMode(InputMode);
	SetShowMouseCursor(false);
}




