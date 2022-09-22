// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "UI/PlayerInfoWidget.h"
#include "PuzzlePlatformsCharacter.h"
#include "PuzzlePlatformsGameInstance.h"
#include "AbilitySystem/UI/HudUpDisplayWidget.h"
#include"PlayersComponent/MyCharacterStatComponent.h"
#include "Soldier.h"
#include "Warrior.h"

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

	static ConstructorHelpers::FClassFinder< UPlayerInfoWidget> UI_HUD_C(TEXT("/Game/PuzzlePlatforms/Widget/WBP_PlayerInfo"));
	if (UI_HUD_C.Succeeded())
	{
		PlayerInfoHUDWidgetClass = UI_HUD_C.Class;
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
	test = 1;
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Controller Beginplay %s %d"), *GetName(), test);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client Controller Beginplay %s %d"), *GetName(), test);
	}
	if (IsLocalController() )
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		//������ ������ beginplay���� �ʱ�ȭ���Ѿߵ�
		//NewHUDWidget = CreateWidget<UUserWidget>(this, NewHUDWidgetClass);


	}
	auto GameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (GameMode==nullptr&&HasAuthority() == true)//�翬 true
	//{
	//	FTimerHandle PossessCharacterHandle;
	//	FTimerDelegate PossessCharacterDelegate = FTimerDelegate::CreateUObject(this, &AMyPlayerController::PossessCharacter);
	//	GetWorldTimerManager().SetTimer(PossessCharacterHandle, PossessCharacterDelegate, .2f, false);
	//}

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


void AMyPlayerController::SetWidget()
{
	//if (IsLocallyControlled() && IsPlayerControlled())
	{


		PlayerInfoHUDWidget = CreateWidget<UPlayerInfoWidget>(GetWorld(), PlayerInfoHUDWidgetClass);
		if (PlayerInfoHUDWidget != nullptr)
		{
			PlayerInfoHUDWidget->AddToViewport();
		}
		//�̰� ability
		auto HeadUpDispaly = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->GetHeadsUpDisplay();
		if (HeadUpDispaly != nullptr)
			HeadUpDispaly->AddToViewport();
		HasWidget = true;
	}
}

void AMyPlayerController::BindWidget(class UMyCharacterStatComponent* NewCharacterStat)
{
	if (PlayerInfoHUDWidget != nullptr)
	{
		//Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
		PlayerInfoHUDWidget->BindCharacterStat(NewCharacterStat);
		auto MyGameInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
	}
}

