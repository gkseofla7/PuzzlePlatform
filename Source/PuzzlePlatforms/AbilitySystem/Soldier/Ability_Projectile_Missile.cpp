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
	DamageAmount = 50;
	if (PlayerRef->IsLocallyControlled() == true)//�����ϴ»����
	{

		SoldierRef = Cast<ASoldier>(PlayerRef);
		SoldierRef->ShowPath = true;
		//SoldierRef->SetUsingSkill(true);
		SoldierRef->GridSphere->SetVisibility(true, true);
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_Projectile_Missile::ActivateEffect);//����ϴ¾ֵ鸸
	}
}

void AAbility_Projectile_Missile::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();

	PlayerRef->OnSkillReleased.Clear();//Release�ϴ� ���� �ʱ�ȭ
	if (PlayerRef->IsLocallyControlled() == false)
		return;//���ʿ� ���⿡ ������ ����

	PlayerRef->SetUsingSkill(false);
	SoldierRef->ClearPointsArray();
	SoldierRef->GridSphere->SetVisibility(false, true);
	SoldierRef->GetMesh()->bPauseAnims = false;
	SoldierRef->ShowPath = false;
	//SoldierRef->SetUsingSkill(false);
	Server_SetVisibility();
	Server_DetachAbilityFromPlayer();//��� �ϴ� ��
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
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);//�Ƹ� �̰� Character
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//�Ƹ� �̰� Pawn
		TArray<AActor*> OutActors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 500., ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);
		UE_LOG(LogTemp, Warning, TEXT("OverlapActors : %d"), OutActors.Num());
		for (int i = 0; i < OutActors.Num(); i++)
		{
			auto Player = Cast< ACharacter_Master>(OutActors[i]);
			if (Player != nullptr)
			{//�� ���� ���̸� �ȸ°���
				if (Player->TeamNum == PlayerRef->TeamNum)//�������̸� ������X
				{
					continue;
				}
			}
			FDamageEvent DamageEvent;
			if (OutActors[i] != nullptr)
			{
				UGameplayStatics::ApplyDamage(OutActors[i], DamageAmount, PlayerRef->GetController(), PlayerRef, UDamageType::StaticClass());

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
	//�������� �ҷ���
	auto Spells = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), Spells.Num());
	for (int i = 0; i < Spells.Num(); i++)
	{
		if (Spells[i]->IsChildOf(AAbility_Projectile_Missile::StaticClass()) == true)
		{
			AbilityLevel = Cast< AMyPlayerState>(PlayerRef->GetPlayerState())->SpellsUpgrade[i];
		}
	}
}