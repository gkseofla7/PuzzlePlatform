// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PlayerInfoWidget.h"
#include "PuzzlePlatformsCharacter.h"

#include "MyLobbyGameMode.h"
AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder< UPlayerInfoWidget> UI_HUD_C(TEXT("/Game/PuzzlePlatforms/Widget/WBP_PlayerInfo"));
	static ConstructorHelpers::FClassFinder< UUserWidget> NEWUI_HUD_C(TEXT("/Game/AbilitySystem/UI/HeadsUpDisplay"));
	///Game/AbilitySystem/UI/HeadsUpDisplay
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
	if (NEWUI_HUD_C.Succeeded())
	{
		NewHUDWidgetClass = NEWUI_HUD_C.Class;
	}
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}
void AMyPlayerController::OnPossess(APawn* aPawn)
{

	Super::OnPossess(aPawn);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	

	if (IsLocalController() )
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		//위젯은 무조건 beginplay에서 초기화시켜야됨
		HUDWidget = CreateWidget<UPlayerInfoWidget>(this, HUDWidgetClass);
		NewHUDWidget = CreateWidget<UUserWidget>(this, NewHUDWidgetClass);

		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
			NewHUDWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Here Is"));
			auto MyPawn = Cast<APuzzlePlatformsCharacter>(GetPawn());
			if (MyPawn != nullptr)
			{
				HUDWidget->BindCharacterStat(MyPawn->CharacterStat);
			}
			
		}
	}
}

UPlayerInfoWidget* AMyPlayerController::GetHudWidget() const
{
	return HUDWidget;
}

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