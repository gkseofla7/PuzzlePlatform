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
			//�̰� ability
			auto HeadUpDispaly = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->GetHeadsUpDisplay();
			if (HeadUpDispaly != nullptr)
				HeadUpDispaly->AddToViewport();
		}

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
		if (!(MyGameInstance->PlayerName.EqualTo(FText::GetEmpty())))
		{//�̸����
			if (PlayerInfoHUDWidget != nullptr)
				PlayerInfoHUDWidget->BindCharacterName(MyGameInstance->PlayerName);
		}
	}
}

//void AMyPlayerController::PossessCharacter()//�� ���ʿ�.. �������� �ƴ� Ŭ���̾�Ʈ����
//{
//	auto MyInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
//	if (MyInstance != nullptr)
//	{
//		if (MyInstance->CharacterIndex == 1)
//		{
//			TArray<AActor*> AllActors;
//			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoldier::StaticClass(), AllActors);
//			for (auto AllActor : AllActors)
//			{
//				auto Soldier = Cast<ASoldier>(AllActor);
//				if (Soldier->bPossessed == false)
//				{
//					Soldier->bPossessed = true;
//					Possess(Soldier);
//					break;
//				}
//			}
//		}
//
//		else if (MyInstance->CharacterIndex == 2)
//		{
//			TArray<AActor*> AllActors;
//			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWarrior::StaticClass(), AllActors);
//			for (auto AllActor : AllActors)
//			{
//				auto Warrior = Cast<AWarrior>(AllActor);
//				if (Warrior->bPossessed == false)
//				{
//					Warrior->bPossessed = true;
//					Possess(Warrior);
//					break;
//				}
//			}
//		}
//	}
//}
