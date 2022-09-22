// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_TargetMissile.h"
#include "../../Soldier.h"

AAbility_TargetMissile::AAbility_TargetMissile()
	:Super()
{
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
		SoldierRef->SetUsingSkill(true);
		SoldierRef->ShowTarget = true;
		SoldierRef->GetMesh()->bPauseAnims = true;
		PlayerRef->OnSkillReleased.AddUObject(this, &AAbility_TargetMissile::ActivateEffect);//����ϴ¾ֵ鸸
	}

}

void AAbility_TargetMissile::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();


}