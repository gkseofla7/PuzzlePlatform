// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Master.h"


#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "HudUpDisplayWidget.h"
#include "BuffPanel_UI.h"
void AAbility_Buff_Master::BeginPlay()
{
	Super::BeginPlay();
	ClearDuplicates();
	bReplicates = true;


}

void AAbility_Buff_Master::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	SetLifeSpan(BuffLifeSpan);
	ApplyBuff();

}

void AAbility_Buff_Master::ApplyBuff()
{

	FTimerHandle TimerHandler;

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff_Master::TickBuff, BuffTickRate, true);
	if (PlayerRef->IsLocallyControlled() == true)
	{
		HudUI->BuffPanel_UI->AddBufftoUI(this);
	}
}
void AAbility_Buff_Master::ClearBuff()
{

}

void AAbility_Buff_Master::TickBuff()
{

}

void AAbility_Buff_Master::ClearDuplicates()
{
	TArray<AActor*> OutputActors;
	TArray<AActor*> DestoryActors;//혹시몰라서

	PlayerRef->GetAttachedActors(OutputActors);
	UE_LOG(LogTemp, Warning, TEXT("Clear? %f"), OutputActors.Num());
	for (auto Actor : OutputActors)
	{
		if (Actor->GetClass() == GetClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Clear!"));
			//DestoryActors.Add(Actor);
			Actor->Destroy();//아마 상관없을듯?
		}
	}

}

void AAbility_Buff_Master::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnEndBuffDelegate.Broadcast();
}