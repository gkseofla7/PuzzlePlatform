// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnSection.h"
#include "../AI/NPC_Master.h"
#include "../AI/NPC_Goblin.h"
#include "../AI/NPC_Archer.h"
#include "../AI/NPCAIController.h"

#include "NavigationSystem.h"
#include "Components/BoxComponent.h"

// Sets default values
AMonsterSpawnSection::AMonsterSpawnSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetupAttachment(RootComponent);
	EnemySpawnTime = 10.0f;
	//static ConstructorHelpers::FClassFinder<AMissile> MissileBPClass((TEXT("/Game/RocketPath/BP_Missile")));
	//if (MissileBPClass.Succeeded())	MissileClass = MissileBPClass.Class;
}

// Called when the game starts or when spawned
void AMonsterSpawnSection::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() == true)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle,
			FTimerDelegate::CreateUObject(this, &AMonsterSpawnSection::OnNPCSpawn), EnemySpawnTime, true);
	}
}

// Called every frame
void AMonsterSpawnSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterSpawnSection::OnNPCSpawn()
{
	if (Monsters.Num() > 20)
		return;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return;

	FVector Origin = GetActorLocation();
	FNavLocation NextPatrol;
	FVector RandomLoc;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1200, NextPatrol))
	{
		RandomLoc = NextPatrol.Location;
		RandomLoc.Z = 90;

	}
	else
		return;
	UE_LOG(LogTemp, Warning, TEXT("Spawn!!"));
	if (Toggle == false&&NPC_MasterClass1!= nullptr)
	{
		Toggle = true;
		//NPC_MasterClass1.
		FTransform MonsterTransform;
		MonsterTransform.SetLocation(RandomLoc);
		MonsterTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		auto Monster = GetWorld()->SpawnActor <ANPC_Master>(NPC_MasterClass1.Get(), MonsterTransform);
		if (Monster == nullptr)
		{
			OnNPCSpawn();
			Toggle = true;
		}
		auto NPCController = Cast<ANPCAIController>(Monster->GetController());
		ABCHECK(NPCController != nullptr);
		NPCController->SetHomePosKey(GetActorLocation());
		//GetWorld()->SpawnActor <ANPC_Master>(GetActorLocation() + FVector::UpVector * 90,r);
	}
	else if(NPC_MasterClass2 != nullptr)
	{
		Toggle = false;
		FTransform MonsterTransform;
		MonsterTransform.SetLocation(RandomLoc);
		MonsterTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		auto Monster = GetWorld()->SpawnActor <ANPC_Master>(NPC_MasterClass2.Get(), MonsterTransform);
		if (Monster == nullptr)
		{
			OnNPCSpawn();
			Toggle = true;
		}
		auto NPCController = Cast<ANPCAIController>(Monster->GetController());
		ABCHECK(NPCController != nullptr);
		NPCController->SetHomePosKey(GetActorLocation());
	}
}
