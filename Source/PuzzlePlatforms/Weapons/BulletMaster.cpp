// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMaster.h"
#include "../Soldier.h"
#include "../Turret/Turret.h"

#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "../Character_Master.h"
// Sets default values
ABulletMaster::ABulletMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//StaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("StaticMesh"));
	RootComponent = Capsule;
	bReplicates = true;
	//StaticMesh->SetupAttachment(Capsule);

}

// Called when the game starts or when spawned
void ABulletMaster::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABulletMaster::OnOverlapBegin);
	Shooter = Cast<ASoldier>(GetInstigator());
	//Capsule->OnComponentHit.AddDynamic(this, &ABulletMaster::OnHit);
	
}

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

	if (OtherActor == Shooter)
		return;
	//UE_LOG(LogTemp, Warning, TEXT("Attacking Actor : %s"), *Shooter->GetName());
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

				//여기서 컨트롤러가..ㅋㅋ 다른 서버쪽 기준 컨트롤러로 돼있을텐데


			}
			FDamageEvent DamageEvent;
			//OtherActor->TakeDamage(10.0f, DamageEvent, MyCharacter->GetController(), Shooter);
			OtherActor->TakeDamage(10.0f, DamageEvent,nullptr, Shooter);
			//UGameplayStatics::ApplyDamage(MyCharacter, 10,nullptr, nullptr,UDamageType::);
			//MyCharacter->ApplyDamage();
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
		}
		this->Destroy();
	}
}
