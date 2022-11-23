// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "UI/PlayerInfoWidget.h"
#include "Character_Master.h"
#include "PuzzlePlatformsGameInstance.h"
#include "AbilitySystem/UI/HudUpDisplayWidget.h"
#include"PlayersComponent/MyCharacterStatComponent.h"
#include "Soldier.h"
#include "Warrior.h"
#include "MyPlayerState.h"

#include "MyLobbyGameMode.h"
#include "Net/UnrealNetwork.h"
AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;


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
	}
	auto GameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTimeSync(DeltaTime);
}


void AMyPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)//5초마다 한번씩
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AMyPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AMyPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest +SingleTripTime ;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}


void AMyPlayerController::SetInputModeGameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture (true);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void AMyPlayerController::SetInputModeGameUI()
{
	FInputModeUIOnly InputMode;
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

float AMyPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

//
//void AMyPlayerController::SetWidget()
//{
//	//if (IsLocallyControlled() && IsPlayerControlled())
//	
//	if (HasWidget == true)//Widget아직 안열려있으면 widget viewport함, 죽었다 살아났다해서
//		return;
//
//
//	PlayerInfoHUDWidget = CreateWidget<UPlayerInfoWidget>(GetWorld(), PlayerInfoHUDWidgetClass);
//	if (PlayerInfoHUDWidget != nullptr)
//	{
//		PlayerInfoHUDWidget->AddToViewport();
//	}
//		//이게 ability
//	auto HeadUpDispaly = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->GetHeadsUpDisplay();
//	if (HeadUpDispaly != nullptr)
//		HeadUpDispaly->AddToViewport();
//	HasWidget = true;
//	
//}

void AMyPlayerController::SetWidget(class UMyCharacterStatComponent* NewCharacterStat)
{

	if (PlayerInfoHUDWidget==nullptr)
	{

		PlayerInfoHUDWidget = Cast< UPlayerInfoWidget>(CreateWidget<UUserWidget>(this, PlayerInfoHUDWidgetClass));
		if (PlayerInfoHUDWidget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("In GameInstance Init nullptr"));
		}
	}
	//PlayerInfoHUDWidget = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance())->PlayerInfoWidget;
	//PlayerInfoHUDWidget->AddToViewport();
	if (PlayerInfoHUDWidget != nullptr&&PlayerInfoHUDWidget->IsInViewport() ==false)
	{
		PlayerInfoHUDWidget->AddToViewport();
	}
	if (PlayerInfoHUDWidget != nullptr)
	{
		//Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
		PlayerInfoHUDWidget->BindCharacterStat(NewCharacterStat);
		auto MyGameInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
		auto MyPlayerState = GetPlayerState<AMyPlayerState>();
		if(MyPlayerState!= nullptr)
			PlayerInfoHUDWidget->BindCharacterName(FText::FromString(MyPlayerState->GetPlayerName()));//내 HP MP 위젯에 이름 등록
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

