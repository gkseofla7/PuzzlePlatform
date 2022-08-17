// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMaster.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "../PuzzlePlatformsCharacter.h"
// Sets default values
ABulletMaster::ABulletMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	//StaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("StaticMesh"));
	RootComponent = Capsule;
	//StaticMesh->SetupAttachment(Capsule);

}

// Called when the game starts or when spawned
void ABulletMaster::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentHit.AddDynamic(this, &ABulletMaster::OnHit);
	
}

// Called every frame
void ABulletMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletMaster::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit!"));
	if (HasAuthority())
	{

		auto MyCharacter = Cast<APuzzlePlatformsCharacter>(OtherActor);
		if (MyCharacter != nullptr)
		{
			FDamageEvent DamageEvent;
			//여기서 컨트롤러가..ㅋㅋ 다른 서버쪽 기준 컨트롤러로 돼있을텐데
			MyCharacter->TakeDamage(10.0f, DamageEvent, MyCharacter->GetController(), this);
			//UGameplayStatics::ApplyDamage(MyCharacter, 10,nullptr, nullptr,UDamageType::);
			//MyCharacter->ApplyDamage();
		}
	}
	this->Destroy();
}