// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ABot::ABot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	GunComponent->SetSimulatePhysics(false);
	GunComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GunComponent->SetupAttachment(GetMesh(), "Weapon");
	//GunMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");

	//RootComponent =GunMeshComponent;

	//auto MyMesh = GetMesh();

}

// Called when the game starts or when spawned
void ABot::BeginPlay()
{
	Super::BeginPlay();

	/*GunMeshComponent->AttachToComponent(GetMesh()
		, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("hand_rSocket"));*/
}

// Called every frame
void ABot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

