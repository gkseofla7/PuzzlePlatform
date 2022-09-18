// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterSection.h"
#include "../LobbyCharacter.h"
#include "../PuzzlePlatformsGameInstance.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
ASelectCharacterSection::ASelectCharacterSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetVisibility(true);
}

// Called when the game starts or when spawned
void ASelectCharacterSection::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASelectCharacterSection::OnBoxBeginOverlap);
}

// Called every frame
void ASelectCharacterSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectCharacterSection::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	auto MyPlayer = Cast< ALobbyCharacter>(OtherActor);
	if (MyPlayer != nullptr&&MyPlayer->IsLocallyControlled()&&MyPlayer->IsPlayerControlled())
	{
		auto MyGameInstance = Cast< UPuzzlePlatformsGameInstance>(GetGameInstance());
		ABCHECK(MyGameInstance);
		MyGameInstance->CharacterIndex = CharacterIndex;
	}
}