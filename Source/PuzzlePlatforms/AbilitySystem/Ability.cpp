// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "../PuzzlePlatformsCharacter.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "HudUpDisplayWidget.h"
#include "../AnimInstance/AnimInstance_Master.h"
#include "Ability_Buff_Master.h"

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
	
	PlayerRef = Cast<APuzzlePlatformsCharacter>(GetOwner());
	AnimRef = Cast<UAnimInstance_Master>(PlayerRef->GetMesh()->GetAnimInstance());
	auto IsBuff = Cast<AAbility_Buff_Master>(this);

	//AnimRef->IsAttacking = true;

	if (PlayerRef->IsLocallyControlled() == true)
	{
		PlayerRef->SetUsingSkill(true);
		//if (IsBuff == nullptr)
		//	PlayerRef->SetIsAttacking(true);
		//UE_LOG(LogTemp, Warning, TEXT("Binding AnimEnd"));
		AnimRef->OnMontageEnded.AddDynamic(this, &AAbility::EndAnimation);
	}
		HudUI =
			Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->HeadsUpDisplay;
		if(PlayerRef->IsLocallyControlled())
			BeginCasting();
	
	

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
		CastBar = HudUI->DisplayCastBar(this);
		//이거에서 중복되는구나..

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
	if (CastBar != nullptr)
	{
		CastBar->CastSuccessful.Clear();
	}
	CastSuccessDeleagate.Broadcast();
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
}

void AAbility::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("EndAnimation"));
	//PlayerRef->SetIsAttacking(false);
	PlayerRef->SetUsingSkill(false);
	AnimationEnd = true;
	if (NeedToDestroy == true)
	{
		Destroy();
	}
}