// Fill out your copyright notice in the Description page of Project Settings.


#include "CharDamageText.h"
#include "../Character_Master.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACharDamageText::ACharDamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharDamageText::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABCHECK(MyController != nullptr);
	auto MyPawn = Cast< ACharacter_Master>(MyController->GetPawn());
	if (MyPawn == nullptr)
		return;
	auto Dir = MyPawn->FollowCamera->GetComponentLocation() - GetActorLocation();
	auto DirRot = UKismetMathLibrary::MakeRotFromX(Dir);
	SetActorRotation(DirRot);
}

