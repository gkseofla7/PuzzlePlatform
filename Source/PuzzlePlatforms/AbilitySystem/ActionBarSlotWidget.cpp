// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarSlotWidget.h"


#include "Ability.h"
#include"../PuzzlePlatformsCharacter.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
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
}
bool UActionBarSlotWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(CastButton != nullptr)) return false;
	CastButton->OnClicked.AddDynamic(this, &UActionBarSlotWidget::CastButtonClicked);



	return true;
}

void UActionBarSlotWidget::CastButtonClicked()
{
	FTransform PlayerTransform = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorTransform();
	FActorSpawnParameters Params;
	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform);

	ability->PlayerRef = Cast<APuzzlePlatformsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//UWorld::SpawnActor(AbilityClass, PlayerTransform);
	//GetWorld()->SpawnActor();
}

void UActionBarSlotWidget::AbilitySpawn(APuzzlePlatformsCharacter* NewPlayer)
{
	if (!NewPlayer->HasAuthority())
		return;
	UE_LOG(LogTemp, Warning, TEXT("Spawn In Server"));
	FTransform PlayerTransform = NewPlayer->GetActorTransform();
	FActorSpawnParameters Params;
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner = NewPlayer;
	SpawnInfo.Instigator = NewPlayer;
	//auto ability = GetWorld()->SpawnActorDeferred<AAbility>(AbilityClass, PlayerTransform,NewPlayer,NewPlayer);
	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform, SpawnInfo);

		//ability->PlayerRef = NewPlayer;//컨트롤러가 있는 플레이어
	//UWorld::SpawnActor(AbilityClass, PlayerTransform);
	//GetWorld()->SpawnActor();
}