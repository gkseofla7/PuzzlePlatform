// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "PlayerInfoWidget.h"
#include "PuzzlePlatformsCharacter.h"

#include "MyLobbyGameMode.h"
AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder< UPlayerInfoWidget> UI_HUD_C(TEXT("/Game/PuzzlePlatforms/Widget/WBP_PlayerInfo"));
	
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, LocationX : %.3f"), *GetName(), 1,3);
	UE_LOG(PuzzlePlatforms, Warning, TEXT("Actor Name "));
}
void AMyPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
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
		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
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