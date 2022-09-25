// Fill out your copyright notice in the Description page of Project Settings.


#include "Object_Master.h"

#include "../Character_Master.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AObject_Master::AObject_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SkeletalMeshComponent =  CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

// Called when the game starts or when spawned
void AObject_Master::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObject_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObject_Master::AttachToPlayer(ACharacter_Master* NewPlayer, FName SocketName)
{
	Player = NewPlayer;
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FAttachmentTransformRules ItemTransformRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Player->GetMesh(), ItemTransformRules, SocketName);

}