// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSection.h"
#include "../Character_Master.h"

#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARespawnSection::ARespawnSection()
	:Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RecoverySpace = CreateDefaultSubobject<USphereComponent>(TEXT("RecoverySpace"));
	RecoverySpace->SetupAttachment(RootComponent);

	RecoverySpace->OnComponentBeginOverlap.AddDynamic(this, &ARespawnSection::OnOverlapBegin);
	RecoverySpace->OnComponentEndOverlap.AddDynamic(this, &ARespawnSection::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ARespawnSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ARespawnSection::GetRandomTransform()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoNavSystem"));
		FTransform out;
		return out;
	}
	FVector Origin = GetActorLocation();
	FNavLocation NextPatrol;
	FVector RandomLoc;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500, NextPatrol))
	{
		RandomLoc = NextPatrol.Location;
		RandomLoc.Z += 200;

	}
	FTransform OutTransform;
	//if (int(RandomLoc.X) == 0)
	//{
	//	RandomLoc.X = GetActorLocation()
	//}
	OutTransform.SetLocation(RandomLoc);
	
	
	UE_LOG(LogTemp, Warning, TEXT("WTFISHappening %f, %f"), RandomLoc.X, RandomLoc.Y);

	if (int(RandomLoc.X) == 0)
	{
		return GetActorTransform();
	}
	else
		return OutTransform;
}

void ARespawnSection::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello"));

	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin %s, %s %s"), *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());

	
	auto Player = Cast<ACharacter_Master>(OtherActor);
	if (Player != nullptr&& Player->IsLocallyControlled())
	{
		if (Player->TeamNum == TeamNum)//快府评捞搁
		{
			Player->bIsInRespawnSection = true;
		}
	}
	
}

void ARespawnSection::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd %s, %s %s"), *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());


	auto Player = Cast<ACharacter_Master>(OtherActor);
	if (Player != nullptr&& Player->IsLocallyControlled())
	{
		if (Player->TeamNum == TeamNum)//快府评捞搁
		{
			Player->bIsInRespawnSection = false;
		}
	}
	
}