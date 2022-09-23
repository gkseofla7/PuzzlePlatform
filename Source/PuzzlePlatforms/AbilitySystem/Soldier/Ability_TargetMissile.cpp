// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_TargetMissile.h"
#include "../../Soldier.h"
#include "TargetMissileMovementComponent.h"
#include "../../Weapons/Weapon_Master.h"

AAbility_TargetMissile::AAbility_TargetMissile()
	:Super()
{
	MissileMovementComponent = CreateDefaultSubobject<UTargetMissileMovementComponent>(TEXT("MissileMovementComponent"));
	
	MissileComponent = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("MissileComponent"));
	MissileComponent->SetupAttachment(RootComponent);
	MissileComponent->SetVisibility(false);
}
void AAbility_TargetMissile::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerRef->IsLocallyControlled() == true)//�����ϴ»����
	{

		SoldierRef = Cast<ASoldier>(PlayerRef);
		SoldierRef->ShowTarget = true;
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_TargetMissile::ActivateEffect);//����ϴ¾ֵ鸸
	}



}

void AAbility_TargetMissile::ActivateEffect_Implementation()
{
	//Super::ActivateEffect_Implementation();

	PlayerRef->OnSkillReleased.Clear();//Release�ϴ� ���� �ʱ�ȭ
	if (SoldierRef->IsLocallyControlled() == false)
		return;//���ʿ� ���⿡ ������ ����

	SoldierRef->SetUsingSkill(false);
	SoldierRef->GetMesh()->bPauseAnims = false;
	SoldierRef->ShowTarget = false;
	//SoldierRef->SetUsingSkill(false);
	Server_SetTransform(SoldierRef->EquippedItem->SkeletalMeshComponent->GetSocketTransform("Muzzle"));
	Server_SetVisibility();
	Server_DetachAbilityFromPlayer();//��� �ϴ� ��

	MissileMovementComponent->Target = SoldierRef->CurrentTarget;
	MissileMovementComponent->bActivate = true;
}

void AAbility_TargetMissile::Server_SetVisibility_Implementation()
{
	NetMulticast_SetVisibility();
}

void AAbility_TargetMissile::NetMulticast_SetVisibility_Implementation()
{
	MissileComponent->SetVisibility(true);
}


bool AAbility_TargetMissile::Server_SetVisibility_Validate()
{
	return true;
}

bool AAbility_TargetMissile::NetMulticast_SetVisibility_Validate()
{
	return true;
}