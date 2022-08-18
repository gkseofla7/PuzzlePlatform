// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBase"));
	StaticMeshTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshTop"));
	StaticMeshBarrelL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBarrelL"));
	StaticMeshBarrelR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBarrelR"));
	RootComponent = StaticMeshBase;
	StaticMeshTop->SetupAttachment(StaticMeshBase,"TopSocket");
	StaticMeshBarrelL->SetupAttachment(StaticMeshTop, "LeftBarrelSocket");
	StaticMeshBarrelR->SetupAttachment(StaticMeshTop, "RightBarrelSocket");
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::FindBestTarget()
{
	float BestDistance = SensingRange + 1;
	AActor* BestTarget = nullptr;
	AActor* CurrentTarget = nullptr;
	float CurrentDistance = 0;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	IgnoreActors.Add(this);
	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SensingRange, ObjectTypes, nullptr, IgnoreActors, OutActors);

	if (isOverlapped)
	{

		for (int i = 0; i < OutActors.Num(); i++)
		{
			
				
			CurrentTarget = OutActors[i];
			CurrentDistance = CurrentTarget->GetDistanceTo(this);
			if (CurrentDistance < BestDistance || BestTarget==nullptr)
			{
				
				TArray<AActor*> IgnoreActorsLinetrace;
				IgnoreActorsLinetrace.Add(this);
				IgnoreActorsLinetrace.Add(CurrentTarget);

				bool tmp = HasLineOfSight(StaticMeshTop->GetComponentLocation(), CurrentTarget->GetActorLocation(), IgnoreActorsLinetrace);
				UE_LOG(LogTemp, Warning, TEXT("For Test"));
				if (tmp == true)
				{
					
					BestTarget = CurrentTarget;
					BestDistance = CurrentDistance;
				}
			}
		}
	}
	Target = BestTarget;
}

bool ATurret::HasLineOfSight(FVector From, FVector To, TArray<AActor*> ActorsToIgnore)
{
//	UKismetSystemLibrary::LineTraceSingle	= > 내부적으로는 	UWorld::LineTraceSingleByChannel
	//FVector Target = AimObejctFPP->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	return !(GetWorld()->LineTraceSingleByChannel(Hit, From, To, ECC_Visibility, QueryParams));
}
