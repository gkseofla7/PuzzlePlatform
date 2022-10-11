// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SupportSoldier.h"
#include "../../AI/Robot/Bot.h"
#include "../../Character_Master.h"

AAbility_SupportSoldier::AAbility_SupportSoldier()
	:Super()
{
	bReplicates = true;
	static ConstructorHelpers::FClassFinder<ABot> BOTBPClass(TEXT("/Game/AI/Bot/BP_Bot"));
	BotClass = BOTBPClass.Class;
}

void AAbility_SupportSoldier::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef->SetUsingSkill(false);
	if (HasAuthority())
	{
		SpawnSoldierBot();
	}

}


void AAbility_SupportSoldier::SpawnSoldierBot()
{
	FTransform PlayerTransform = GetOwner()->GetActorTransform();
	FActorSpawnParameters Params;
	FActorSpawnParameters SpawnInfo;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerRef->GetName());
	SpawnInfo.Owner = Cast<AActor>(PlayerRef);
	SpawnInfo.Instigator = Cast<APawn>(PlayerRef);
	auto bot = GetWorld()->SpawnActor<ABot>(BotClass, PlayerTransform, SpawnInfo);
}
