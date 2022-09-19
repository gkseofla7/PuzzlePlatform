// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Master.h"


#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "UI/HudUpDisplayWidget.h"
#include "UI/BuffPanel_UI.h"
#include "Ability_Debuff.h"
void AAbility_Buff_Master::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	AbilityRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ClearDuplicates();
	bReplicates = true;
	//FTimerHandle TimerHandler;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff_Master::TickBuff, BuffLifeSpan, false);

}

void AAbility_Buff_Master::Tick(float DeltaTime)
{
	TimeSpend += DeltaTime;
	if (TimeSpend > 10.)
	{
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
	//if (HasAuthority())
	//{
	//	
	//	SetLifeSpan(BuffLifeSpan);
	//}
	//else
	//{
	//}

	if (PlayerRef->IsLocallyControlled() == true)//어차피 나 자신
	{
		HudUI->BuffPanel_UI->AddBufftoUI(this);
	}
	Server_ApplyBuff();


}

void AAbility_Buff_Master::Server_ApplyBuff_Implementation()
{
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff_Master::TickBuff, BuffTickRate, bIsRepeat);
}

bool AAbility_Buff_Master::Server_ApplyBuff_Validate()
{
	return true;
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
	OnEndBuffDelegate.Broadcast();
}