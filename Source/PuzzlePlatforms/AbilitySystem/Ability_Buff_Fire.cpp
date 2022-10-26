// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Fire.h"
#include "../Character_Master.h"

AAbility_Buff_Fire::AAbility_Buff_Fire()
	:Super()
{
	DamageAmount = 2.f;
}

void AAbility_Buff_Fire::BeginPlay()
{
	Super::BeginPlay();
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "spine_01Socket");
}



void AAbility_Buff_Fire::TickBuff()
{
	Super::TickBuff();
	if (HasAuthority())
	{
		UGameplayStatics::ApplyDamage(PlayerRef, DamageAmount, PlayerRef->GetController(), this, UDamageType::StaticClass());
	}
}

