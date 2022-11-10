// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Missile.h"
#include "../../Soldier.h"
#include "../ActorAbilities.h"

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
	DamageAmount = 10;
	if (PlayerRef->IsLocallyControlled() == true)//조종하는사람만
	{

		SoldierRef = Cast<ASoldier>(PlayerRef);
		SoldierRef->Direction = 0;
		SoldierRef->ShowPath = true;
		//SoldierRef->SetbUsingSkill(true);
		SoldierRef->GridSphere->SetVisibility(true, true);
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_Projectile_Missile::ActivateEffect);//등록하는애들만
	}
}

void AAbility_Projectile_Missile::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();

	PlayerRef->OnSkillReleased.Clear();//Release하는 순간 초기화
	if (PlayerRef->IsLocallyControlled() == false)
		return;//애초에 여기에 올일은 없음

	PlayerRef->SetbUsingSkill(false);
	SoldierRef->ClearPointsArray();
	SoldierRef->GridSphere->SetVisibility(false, true);
	SoldierRef->GetMesh()->bPauseAnims = false;
	SoldierRef->ShowPath = false;
	//SoldierRef->SetbUsingSkill(false);
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
		UE_LOG(LogTemp, Warning, TEXT("OverlapActors : %d"), OutActors.Num());
		for (int i = 0; i < OutActors.Num(); i++)
		{
			auto Player = Cast< ACharacter_Master>(OutActors[i]);
			if (Player != nullptr)
			{//걍 같은 팀이면 안맞게함
				if (Player->TeamNum == PlayerRef->TeamNum)//같은팀이면 데미지X
				{
					continue;
				}
			}
			FDamageEvent DamageEvent;
			if (OutActors[i] != nullptr)
			{
				UGameplayStatics::ApplyDamage(OutActors[i], DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());
				ACharacter* myCharacter = Cast<ACharacter>(OutActors[i]);
				//if(myCharacter!= nullptr)
				//{
				//	FVector impulse = myCharacter->GetActorLocation() - GetActorLocation();
				//	impulse = 1000 * impulse;
				//	//myCharacter->GetMesh()->SetSimulatePhysics(true);
				//	myCharacter->GetMesh()->AddImpulseAtLocation(impulse, myCharacter->GetActorLocation());//Chracter가 아니면..
				//	
				//	
				//}
				
			}
		}
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

void AAbility_Projectile_Missile::SetAbilityLevel()
{
	//쓸때마다 불러옴
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_Projectile_Missile::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
		}
	}

	if (AbilityLevel == 1)
	{
		DamageAmount = 30.;//0.5초당 2
	}
	else if (AbilityLevel == 2)
	{
		DamageAmount = 50;
	}
	else if (AbilityLevel == 3)
	{
		DamageAmount = 100;
	}
	else if (AbilityLevel == 4)
	{
		DamageAmount = 150;
	}
	else if (AbilityLevel == 5)//5를 맥스로
	{
		DamageAmount = 250;
	}
}