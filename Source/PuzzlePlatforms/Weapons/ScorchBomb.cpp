// Fill out your copyright notice in the Description page of Project Settings.


#include "ScorchBomb.h"
#include "../AI/NPC_Master.h"
#include "../Character_Master.h"
#include "../AI/MonsterStatComponent.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AScorchBomb::AScorchBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	RootComponent = Sphere;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AScorchBomb::BeginPlay()
{
	Super::BeginPlay();
	
	//FVector Dir = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Target->GetActorLocation());
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AScorchBomb::OnOverlapBegin);
	//ProjectileMovementComponent->Velocity = Dir * 1000;
}

// Called every frame
void AScorchBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScorchBomb::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	auto Monster = Cast<ANPC_Master>(GetOwner());
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
				MyCharacter->TakeDamage(Monster->MonsterStat->AttackDamage, DamageEvent, MyCharacter->GetController(), this);
				//UGameplayStatics::ApplyDamage(MyCharacter, 10,nullptr, nullptr,UDamageType::);
				//MyCharacter->ApplyDamage();
			}
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetTransform());
		}
		if (HasAuthority())
			this->Destroy();
	}
}