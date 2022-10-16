// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_AoE_HolyGround.h"
#include "../ActorAbilities.h"
#include "../../Character_Master.h"

#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"

AAbility_AoE_HolyGround::AAbility_AoE_HolyGround()
{
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DECAL"));
	DecalComponent->SetupAttachment(RootComponent);
	
}

void AAbility_AoE_HolyGround::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();
	//DecalComponent->SetVisibility(true);
	


	if (HasAuthority())
	{
		TArray<AActor*>OverlappingActors;
		AbilityRoot->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

		for (auto Actors : OverlappingActors)
		{
			auto Player = Cast < ACharacter>(Actors);
			if (Player == nullptr)
				continue;
			auto PuzzleCharacter = Cast<ACharacter_Master>(Actors);
			if (PuzzleCharacter != nullptr)
			{
				if (PuzzleCharacter == PlayerRef||PuzzleCharacter->TeamNum==PlayerRef->TeamNum)//자기 자신이던가 같은팀이면
					continue;
			}
			FVector OposDir = ( Player->GetActorLocation() - GetActorLocation()).GetSafeNormal() + FVector(0, 0, 500);
			Player->LaunchCharacter(OposDir * 3000, false, false);
			UGameplayStatics::ApplyDamage(Player, DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

		}

	}
	SetLifeSpan(LifeSpan);


//	FTimerHandle TimerHandler;
	
	//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AAbility_AoE_HolyGround::TickEffect, DoTTimer, true);
//	GetWorldTimerManager().SetTimer(TimerHandler,1.,false);
	//UKismetSystemLibrary::K2_SetTimerDelegate()
}

void AAbility_AoE_HolyGround::TickEffect()
{
	//여기서 힐
	TArray<AActor*>OverlappingActors;

	AbilityRoot->GetOverlappingActors(OverlappingActors, ACharacter_Master::StaticClass());

	//if (HasAuthority())
	//{
	//	for (auto Actors : OverlappingActors)
	//	{
	//		UGameplayStatics::ApplyDamage(Actors, DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

	//	}
	//}
}

void AAbility_AoE_HolyGround::SetAbilityLevel()
{
	//쓸때마다 불러옴
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_AoE_HolyGround::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
		}
	}
}