// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Missile.h"
#include "../../Soldier.h"

AAbility_Projectile_Missile::AAbility_Projectile_Missile()
	:Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilityRoot->OnComponentBeginOverlap.AddDynamic(this, &AAbility_Projectile_Missile::OnOverlapBegin);
	MissileComponent = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("MissileComponent"));
	MissileComponent->SetupAttachment(RootComponent);
	MissileComponent->SetVisibility(false);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Etcs/p_Turret_Explosion"));
	if (ParticleAsset.Succeeded())
	{
		ParticleTemplate = ParticleAsset.Object;
	}
}

void AAbility_Projectile_Missile::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerRef->IsLocallyControlled() == true)
	{

		SoldierRef = Cast<ASoldier>(PlayerRef);
		SoldierRef->ShowPath = true;
		//SoldierRef->SetUsingSkill(true);
		SoldierRef->GridSphere->SetVisibility(true, true);
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_Projectile_Missile::ActivateEffect);
	}
}

void AAbility_Projectile_Missile::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();
}
void AAbility_Projectile_Missile::ActivateEffect_Implementation()
{


	PlayerRef->OnSkillReleased.Clear();//Release하는 순간 초기화
	if (PlayerRef->IsLocallyControlled() == false)
		return;//애초에 여기에 올일은 없음

	Super::ActivateEffect_Implementation();
	PlayerRef->SetUsingSkill(false);
	SoldierRef->ClearPointsArray();
	SoldierRef->GridSphere->SetVisibility(false, true);
	SoldierRef->GetMesh()->bPauseAnims = false;
	SoldierRef->ShowPath = false;
	//SoldierRef->SetUsingSkill(false);
	Server_SetVisibility();
	Server_DetachAbilityFromPlayer();//모두 일단 띄어냄
	Server_SetVelocity(SoldierRef->MissileVelocity);
	Server_SetTransform(SoldierRef->RocketHolderComponent->GetSocketTransform("Mouth"));
	Server_Activate();
}


void AAbility_Projectile_Missile::Server_SetVisibility_Implementation()
{
	NetMulticast_SetVisibility();
}

void AAbility_Projectile_Missile::NetMulticast_SetVisibility_Implementation()
{
	MissileComponent->SetVisibility(true);
}


bool AAbility_Projectile_Missile::Server_SetVisibility_Validate()
{
	return true;
}

bool AAbility_Projectile_Missile::NetMulticast_SetVisibility_Validate()
{
	return true;
}

void AAbility_Projectile_Missile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
		return;
	//UE_LOG(LogTemp, Warning, TEXT("Hit %s, %s %s"), *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("FIRST"));
	if (HasAuthority() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("InServer"));
		NetMulticast_Spark(GetActorLocation());
		//auto Player = Cast<APuzzlePlatformsCharacter>(OtherActor);
		//if (Player != nullptr)
		//{
		//	UGameplayStatics::ApplyDamage(Player, DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

		//}
		Destroy();
	}

}

void AAbility_Projectile_Missile::NetMulticast_Spark_Implementation(FVector Location)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, Location, FRotator(0, 0, 0));
}

bool AAbility_Projectile_Missile::NetMulticast_Spark_Validate(FVector Location)
{
	return true;
}