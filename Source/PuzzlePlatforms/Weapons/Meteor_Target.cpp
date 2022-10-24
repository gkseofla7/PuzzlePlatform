// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor_Target.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
// Sets default values
AMeteor_Target::AMeteor_Target()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject< USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	DecalComponent = CreateDefaultSubobject< UDecalComponent>(TEXT("DecalMeteorComponent"));
	DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMeteor_Target::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeteor_Target::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

