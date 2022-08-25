// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PuzzlePlatformsGameInstance.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilityRoot = CreateDefaultSubobject<USphereComponent>(TEXT("AbilityRoot"));
	RootComponent = AbilityRoot;
}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();
	//PlayerRef = Cast<APuzzlePlatformsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	//AttachToComponent(PlayerRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
	//HudUI = Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->HeadsUpDisplay;

	//BeginCasting();
	
}

// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbility::BeginCasting()
{
	if (AbilityDetails.CastTime > 0)
	{
		AttachToComponent(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
		PlayerRef = Cast<APuzzlePlatformsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		HudUI = 
			Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->HeadsUpDisplay;

	}
}