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

	if (PlayerRef->IsLocallyControlled() == true)//조종하는사람만
	{

		SoldierRef = Cast<ASoldier>(PlayerRef);
		SoldierRef->ShowTarget = true;
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_TargetMissile::ActivateEffect);//등록하는애들만
	}



}

void AAbility_TargetMissile::ActivateEffect_Implementation()
{
	//Super::ActivateEffect_Implementation();

	PlayerRef->OnSkillReleased.Clear();//Release하는 순간 초기화
	if (SoldierRef->IsLocallyControlled() == false)
		return;//애초에 여기에 올일은 없음

	SoldierRef->SetUsingSkill(false);
	SoldierRef->GetMesh()->bPauseAnims = false;
	SoldierRef->ShowTarget = false;
	//SoldierRef->SetUsingSkill(false);
	Server_SetTransform(SoldierRef->EquippedItem->SkeletalMeshComponent->GetSocketTransform("Muzzle"));
	Server_SetVisibility();
	Server_DetachAbilityFromPlayer();//모두 일단 띄어냄

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