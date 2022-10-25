// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteor.h"
#include "../Character_Master.h"
#include "../AI/NPC_Master.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	
	ParticleSystemComponent->SetupAttachment(SphereComponent);

	DecalComponent = CreateDefaultSubobject< UDecalComponent>(TEXT("DecalMeteorComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnOverlapBegin);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Etcs/p_Turret_Explosion"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTemplate = ParticleAsset.Object;
	}


}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AMeteor::ActiveDecal, 1.f,false);
	//UE_LOG(LogTemp, Warning, TEXT("MeteorBeginplay"));

}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator rt=	UKismetMathLibrary::MakeRotFromX(GetVelocity());
	SetActorRotation(rt);
}

void AMeteor::ActiveDecal()
{
	UE_LOG(LogTemp, Warning, TEXT("Active"));
	DecalComponent->SetVisibility(true);
}

void AMeteor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Monster = Cast<ANPC_Master>(OtherActor);
	auto Meteor = Cast<AMeteor>(OtherActor);
	if (Monster != nullptr||Meteor!= nullptr)
	{
		return;
	}

	if (HasAuthority() == true)
	{

		NetMulticast_Spark(GetActorLocation());
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;//
		UClass* ActorClassFilter = AActor::StaticClass();
		TArray<AActor*> ActorsToIgnore;// TArray<AActor*>& OutActors)
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);//아마 이게 Character
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//아마 이게 Pawn
		TArray<AActor*> OutActors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 500., ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);

		for (int i = 0; i < OutActors.Num(); i++)
		{
			auto Player = Cast< ACharacter_Master>(OutActors[i]);
			if (Player != nullptr)
			{//걍 같은 팀이면 안맞게함
				FDamageEvent DamageEvent;
				if (OutActors[i] != nullptr)
				{
					UGameplayStatics::ApplyDamage(OutActors[i], DamageAmount, nullptr, this, UDamageType::StaticClass());
				}
			}

		}
		Destroy();
	}
}

void AMeteor::NetMulticast_Spark_Implementation(FVector Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, Location, FRotator(0, 0, 0));
}

bool AMeteor::NetMulticast_Spark_Validate(FVector Location)
{
	return true;
}


void AMeteor::NetMulticast_SetSpeed_Implementation(FVector NewVelocity)
{
	ProjectileComponent->Velocity = NewVelocity;
}
bool AMeteor::NetMulticast_SetSpeed_Validate(FVector NewVelocity)
{
	return true;
}
void AMeteor::NetMulticast_SetDecalLocation_Implementation(FVector NewLocation)
{
	DecalComponent->SetWorldLocation(NewLocation);
}
bool AMeteor::NetMulticast_SetDecalLocation_Validate(FVector NewLocation)
{
	return true;
}
