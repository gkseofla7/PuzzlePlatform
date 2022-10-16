// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulItem.h"
#include "Components/SphereComponent.h"
#include "../Character_Master.h"
#include "../AI/NPC_Master.h"
#include "../MyPlayerState.h"
#include "../AI/MonsterStatComponent.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ASoulItem::ASoulItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void ASoulItem::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ACharacter_Master>(GetOwner());
	MonsterRef = Cast<ANPC_Master>(GetInstigator());
	//float duration = UKismetMathLibrary::RandomFloatInRange(0.5, 1.0);
	//FTimerHandle TimerHandler;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASoulItem::InitialSettings, duration, false);
}

void ASoulItem::CustomInitialize(int NewExp)
{
	Exp = NewExp;
}

// Called every frame
void ASoulItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASoulItem::FinishCollectItem()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished"));
		auto MyPlayerState = Cast<AMyPlayerState>(PlayerRef->GetPlayerState());
		MyPlayerState->CharacterStat->Server_SetExp(MyPlayerState->CharacterStat->CurrentExp + Exp);

		PlayerRef = nullptr;
		Destroy();
	}
	
}
