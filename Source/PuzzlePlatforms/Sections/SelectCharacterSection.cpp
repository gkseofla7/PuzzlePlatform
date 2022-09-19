// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterSection.h"
#include "../LobbyCharacter.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../MyLobbyGameMode.h"

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
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASelectCharacterSection::OnBoxEndOverlap);
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
	if (HasAuthority())
	{
		auto MyGameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ABCHECK(MyGameMode);
		MyGameMode->NumberOfReady++;
		if (MyGameMode->NumberOfPlayers == MyGameMode->NumberOfReady && MyGameMode->NumberOfPlayers % 2 == 0)
		{
			//FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AMyLobbyGameMode::StartPlay,);
			MyGameMode->StartGame();
			//GetWorldTimerManager().SetTimer(GameStartTimer, this, &AMyLobbyGameMode::StartGame, 10.0f);

		}
	}
}

void ASelectCharacterSection::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		auto MyGameMode = Cast< AMyLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		ABCHECK(MyGameMode);
		MyGameMode->NumberOfReady--;

	}
}