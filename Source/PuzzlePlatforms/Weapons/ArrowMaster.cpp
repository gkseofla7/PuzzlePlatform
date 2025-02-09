// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowMaster.h"
#include "../Soldier.h"

#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "../Character_Master.h"
#include "../AI/NPC_Master.h"
// Sets default values
AArrowMaster::AArrowMaster()
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
void AArrowMaster::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AArrowMaster::OnOverlapBegin);

	//Capsule->OnComponentHit.AddDynamic(this, &ABulletMaster::OnHit);

}

// Called every frame
void AArrowMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowMaster::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (Cast<ANPC_Master>(OtherActor) != nullptr)
			return;
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
		if(HasAuthority())
			this->Destroy();
	}
}
