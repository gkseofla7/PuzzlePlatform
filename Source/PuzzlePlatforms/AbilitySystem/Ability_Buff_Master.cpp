// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Master.h"


#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "HudUpDisplayWidget.h"
#include "BuffPanel_UI.h"
#include "Ability_Debuff.h"
void AAbility_Buff_Master::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	//FTimerHandle TimerHandler;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff_Master::TickBuff, BuffLifeSpan, false);

}

void AAbility_Buff_Master::Tick(float DeltaTime)
{
	TimeSpend += DeltaTime;
	if (TimeSpend > 10.)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEstroy"));
		Destroy();
	}
}

void AAbility_Buff_Master::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	auto tmp = Cast<AAbility_Debuff>(this);
	if (tmp != nullptr)
	{
		PlayerRef = PlayerRef->TargetPlayer;
	}

	if (HasAuthority())
	{
		
		SetLifeSpan(BuffLifeSpan);
		UE_LOG(LogTemp, Warning, TEXT("Server SetLifeSpan %f"), GetLifeSpan());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client SetLifeSpan %f"), GetLifeSpan());
	}


	ApplyBuff();

}

void AAbility_Buff_Master::ApplyBuff()
{

	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff_Master::TickBuff, BuffTickRate, true);
	
	if (PlayerRef->IsLocallyControlled() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyBuff"));
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

			//DestoryActors.Add(Actor);
			Actor->Destroy();//아마 상관없을듯?
		}
	}

}

void AAbility_Buff_Master::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Warning, TEXT("EndPlay!"));
	OnEndBuffDelegate.Broadcast();
}