// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_AoE_HolyGround.h"

#include "../PuzzlePlatformsCharacter.h"

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
		AbilityRoot->GetOverlappingActors(OverlappingActors, APuzzlePlatformsCharacter::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Server"));
		for (auto Actors : OverlappingActors)
		{
			if (Actors == PlayerRef)
				continue;
			UGameplayStatics::ApplyDamage(Actors, 10, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

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
	//¿©±â¼­ Èú
	TArray<AActor*>OverlappingActors;

	AbilityRoot->GetOverlappingActors(OverlappingActors, APuzzlePlatformsCharacter::StaticClass());

	if (HasAuthority())
	{
		for (auto Actors : OverlappingActors)
		{
			UGameplayStatics::ApplyDamage(Actors, -50, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

		}
	}
}