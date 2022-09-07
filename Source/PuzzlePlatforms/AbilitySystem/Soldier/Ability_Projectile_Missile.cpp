// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Projectile_Missile.h"
#include "../../Soldier.h"

AAbility_Projectile_Missile::AAbility_Projectile_Missile()
	:Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileComponent = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("MissileComponent"));
	MissileComponent->SetupAttachment(RootComponent);
	MissileComponent->SetVisibility(false);
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


	PlayerRef->OnSkillReleased.Clear();//Release�ϴ� ���� �ʱ�ȭ
	if (PlayerRef->IsLocallyControlled() == false)
		return;//���ʿ� ���⿡ ������ ����

	Super::ActivateEffect_Implementation();
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