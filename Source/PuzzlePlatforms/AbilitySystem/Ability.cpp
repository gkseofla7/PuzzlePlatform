// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "HudUpDisplayWidget.h"

#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "CastBarWidget.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilityRoot = CreateDefaultSubobject<USphereComponent>(TEXT("AbilityRoot"));
	RootComponent = AbilityRoot;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();

	if (AbilityDetails.CastTime > 0)
	{


	
		PlayerRef = Cast<APuzzlePlatformsCharacter>(GetOwner());

	
		HudUI =
			Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->HeadsUpDisplay;
		BeginCasting();
	}
	

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
		auto CastBar = HudUI->DisplayCastBar(this);
		CastBar->CastSuccessful.AddDynamic(this, &AAbility::CastAbility);
	}
	else
	{
		CastAbility();
	}
}

void AAbility::InterruptCast()
{
	Destroy();
}


void AAbility::CastAbility()
{
	CastAbility_Implementation();
}
void AAbility::CastAbility_Implementation()
{
	
	
}

void AAbility::ActivateEffect()
{
	ActivateEffect_Implementation();
}
void AAbility::ActivateEffect_Implementation()
{
}

void AAbility::DetachAbilityFromPlayer()
{




	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FRotator NewRotation = PlayerRef->GetActorRotation();

}

