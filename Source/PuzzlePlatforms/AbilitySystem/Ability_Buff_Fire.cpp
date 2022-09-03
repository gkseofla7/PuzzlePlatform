// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Buff_Fire.h"
#include "../PuzzlePlatformsCharacter.h"

AAbility_Buff_Fire::AAbility_Buff_Fire()
	:Super()
{

}

void AAbility_Buff_Fire::BeginPlay()
{
	Super::BeginPlay();
	AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
}
void AAbility_Buff_Fire::TickBuff()
{
	if (HasAuthority())
	{

		UGameplayStatics::ApplyDamage(PlayerRef, DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());
	}
}

