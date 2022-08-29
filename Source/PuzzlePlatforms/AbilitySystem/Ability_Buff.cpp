// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"
#include "HudUpDisplayWidget.h"
#include "BuffPanel_UI.h"
void AAbility_Buff::BeginPlay()
{
	Super::BeginPlay();
	ClearDuplicates();
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());


	//AsPlayerAnimInstance->IsAttacking = true;
	//AsPlayerAnimInstance->OnActivateSpell.AddUObject(this, &AAbility_AoE::ActivateEffect);

	AsPlayerAnimInstance->PlayAoEAttackMontage();

}

void AAbility_Buff::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
	SetLifeSpan(BuffLifeSpan);
	ApplyBuff();

}

void AAbility_Buff::ApplyBuff()
{
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_Buff::TickBuff, BuffTickRate, true);
	HudUI->BuffPanel_UI->AddBufftoUI(this);
}
void AAbility_Buff::ClearBuff()
{

}

void AAbility_Buff::TickBuff()
{

}

void AAbility_Buff::ClearDuplicates()
{
	TArray<AActor*> OutputActors;
	TArray<AActor*> DestoryActors;//혹시몰라서

	PlayerRef->GetAttachedActors(OutputActors);
	UE_LOG(LogTemp, Warning, TEXT("Clear? %f"), OutputActors.Num());
	for(auto Actor : OutputActors)
	{
		if (Actor->GetClass() == GetClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Clear!"));
			//DestoryActors.Add(Actor);
			Actor->Destroy();//아마 상관없을듯?
		}
	}
	
}

void AAbility_Buff::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnEndBuffDelegate.Broadcast();
}