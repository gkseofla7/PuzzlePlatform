// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMaster.h"
#include "../Character_Master.h"
#include "../Soldier.h"
#include "../Turret/Turret.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"

#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABulletMaster::ABulletMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;

	//StaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("StaticMesh"));
	RootComponent = Capsule;
	bReplicates = true;
	//StaticMesh->SetupAttachment(Capsule);

}

// Called when the game starts or when spawned
void ABulletMaster::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.f);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABulletMaster::OnOverlapBegin);
	Shooter = Cast<ASoldier>(GetInstigator());
	//Capsule->OnComponentHit.AddDynamic(this, &ABulletMaster::OnHit);
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithChannel = true;
	PathParams.bTraceWithCollision = true;
	PathParams.DrawDebugTime = 5.f;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	PathParams.LaunchVelocity = GetActorForwardVector() * InitialSpeed;
	PathParams.MaxSimTime = 4.f;
	//PathParams.OverrideGravityZ = 10;
	PathParams.ProjectileRadius = 5.f;
	PathParams.SimFrequency = 30.f;
	PathParams.StartLocation = GetActorLocation();
	PathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PathParams.ActorsToIgnore.Add(this);


	FPredictProjectilePathResult PathResult;
	//UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
}
#if WITH_EDITOR
void ABulletMaster::PostEditChangeProperty(struct FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);
	
	FName PropertyName = Event.Property!=nullptr? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ABulletMaster, InitialSpeed))
	{
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
	}
}
#endif
// Called every frame
void ABulletMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletMaster::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Cast<ASoldier>(OtherActor) == Shooter)
	//	return;

	if (HasAuthority())
	{

		auto MyCharacter = Cast<ACharacter_Master>(OtherActor);
		if (MyCharacter != nullptr)
		{
			FDamageEvent DamageEvent;
			//여기서 컨트롤러가..ㅋㅋ 다른 서버쪽 기준 컨트롤러로 돼있을텐데
			MyCharacter->TakeDamage(10.0f, DamageEvent, MyCharacter->GetController(), this);
			//UGameplayStatics::ApplyDamage(MyCharacter, 10,nullptr, nullptr,UDamageType::);
			//MyCharacter->ApplyDamage();
		}
	}
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
	}
	this->Destroy();
}

void ABulletMaster::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABCHECK(Shooter != nullptr);
	if (OtherActor == Shooter)
		return;
	
	
	//UE_LOG(LogTemp, Warning, TEXT("Debug Sphere"));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		
		if (HasAuthority())
		{
			
			auto MyCharacter = Cast<ACharacter>(OtherActor);
			if (MyCharacter != nullptr)
			{
				auto Player = Cast<ACharacter_Master>(MyCharacter);
				if (Player != nullptr)
				{//걍 같은 팀이면 안맞게함
					if (Shooter!=nullptr&&Player->TeamNum == Shooter->TeamNum)
					{
						if (ImpactParticles)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
						}
						this->Destroy();
						return;
					}
					if (TurretRef != nullptr && Player->TeamNum == TurretRef->TeamNum)
					{
						if (ImpactParticles)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
						}
						this->Destroy();
						return;
					}
				}
			}
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(Shooter->CharacterStatRef->AttackDamage, DamageEvent,nullptr, Shooter);

		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
		}
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 10.f, 10, FColor::Orange, true);
		this->Destroy();
	}
}
