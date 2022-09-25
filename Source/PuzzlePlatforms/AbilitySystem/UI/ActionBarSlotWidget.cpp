// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarSlotWidget.h"


#include "../Ability.h"
#include"../../Character_Master.h"
#include "DragDrop.h"
#include "ActionBarWidget.h"


#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
UActionBarSlotWidget::UActionBarSlotWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	//ConstructorHelpers::FClassFinder<AAbility> AbilityBPClass(TEXT("/Game/AbilitySystem/BP_Ability"));
	//if (!ensure(AbilityBPClass.Class != nullptr)) return;
	//AbilityClass = AbilityBPClass.Class;
}
void UActionBarSlotWidget::NativePreConstruct()
{

	Super::NativePreConstruct();

	if (AbilityClass != nullptr)
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
		AAbility* DefaultActor = Cast<AAbility>(AbilityClass->GetDefaultObject(true));
		IconImage->SetBrushFromTexture(DefaultActor->AbilityDetails.Icon);

	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		
	}
	UpdateAppearance();
}
bool UActionBarSlotWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(CastButton != nullptr)) return false;
	CastButton->OnClicked.AddDynamic(this, &UActionBarSlotWidget::CastButtonClicked);



	return true;
}

void UActionBarSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (CooldownBar->IsVisible() == true)
	{
		float RemaingTime = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), CooldownTimerHandler);
		float ElapsedTime = UKismetSystemLibrary::K2_GetTimerElapsedTimeHandle(GetWorld(), CooldownTimerHandler);

		float Total = RemaingTime + ElapsedTime;
		CooldownBar->SetPercent(UKismetMathLibrary::NormalizeToRange(RemaingTime, 0, Total));
		
	}
	if (IsManaAvailable == false)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		CooldownBar->SetPercent(1);
	}
}

void UActionBarSlotWidget::CastButtonClicked()
{
	FTransform PlayerTransform = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorTransform();
	FActorSpawnParameters Params;
	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform);

	ability->PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//UWorld::SpawnActor(AbilityClass, PlayerTransform);
	//GetWorld()->SpawnActor();
}

//void UActionBarSlotWidget::AbilitySpawn(ACharacter_Master* NewPlayer)
//{
//	if (!NewPlayer->HasAuthority())
//		return;
//	FTransform PlayerTransform = NewPlayer->GetActorTransform();
//	FActorSpawnParameters Params;
//	FActorSpawnParameters SpawnInfo;
//	Owner = NewPlayer;
//	SpawnInfo.Owner = NewPlayer;
//	SpawnInfo.Instigator = NewPlayer;
//	//auto ability = GetWorld()->SpawnActorDeferred<AAbility>(AbilityClass, PlayerTransform,NewPlayer,NewPlayer);
//	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform, SpawnInfo);
//
//		//ability->PlayerRef = NewPlayer;//컨트롤러가 있는 플레이어
//	//UWorld::SpawnActor(AbilityClass, PlayerTransform);
//	//GetWorld()->SpawnActor();
//}
//애초에 ability에 하는데..ㅋㅋ
void UActionBarSlotWidget::StartCooldown()
{
	IsAvailable = false;
	UpdateAppearance();

	auto ability = AbilityClass.GetDefaultObject();
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandler, this, &UActionBarSlotWidget::EndCooldown, ability->AbilityDetails.CoolDown, false);

}

void UActionBarSlotWidget::EndCooldown()
{
	IsAvailable = true;
	UpdateAppearance();
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), CooldownTimerHandler);
}

void UActionBarSlotWidget::UpdateAppearance()
{
	if (IsAvailable == false)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		CooldownBar->SetPercent(1);
	}
	else
	{
		CooldownBar->SetVisibility(ESlateVisibility::Hidden);
		CooldownBar->SetPercent(0);
	}

}

bool UActionBarSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto DragDrop = Cast<UDragDrop>(InOperation);
	if (DragDrop == nullptr)
		return false;
	AbilityClass = DragDrop->AbilityClass;
	IconImage->SetVisibility(ESlateVisibility::Visible);
	AAbility* ability = Cast<AAbility>(AbilityClass->GetDefaultObject());
	IconImage->SetBrushFromTexture(ability->AbilityDetails.Icon);
	IsAvailable = true;
	if(ParentsWidget->CurrentCharacterStat!=nullptr)
		ParentsWidget->BindCharacterStat(ParentsWidget->CurrentCharacterStat);

	return true;
}