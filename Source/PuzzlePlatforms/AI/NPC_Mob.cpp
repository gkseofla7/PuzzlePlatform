// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Mob.h"
#include "NPCMobAIController.h"
#include "NPC_Archer.h"
#include "../Character_Master.h"

#include "MonsterStatComponent.h"
#include "Net/UnrealNetwork.h"

ANPC_Mob::ANPC_Mob()
	:Super()
{
	AIControllerClass = ANPCMobAIController::StaticClass();
}

void ANPC_Mob::Attack()
{
}



float ANPC_Mob::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{

	//ABCHECK(MotionReplicator != nullptr)

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	auto Player = Cast< ACharacter_Master>(DamageCauser);

	auto Archer = Cast<ANPC_Archer>(this);
	if (Archer != nullptr)
	{
		Archer->NetMulticast_SetTarget(Player);
	}

	if (DamageCauser != nullptr)
	{
		FVector OposDir = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
		LaunchCharacter(OposDir * 1000, false, false);//�ڷ� �и�
	}

	GetHelpedFromOthers(Player);//������ ���ܽ��Ѿߵ�
	NetMulticast_DamageImpact(FinalDamage);


	return FinalDamage;
}


void ANPC_Mob::NetMulticast_DamageImpact_Implementation(float Damage)
{

	DamageImpact(Damage);
	ChangeDamageColor();//�̰� ���� ������ ��ߵ�
	if (HasAuthority())
	{
		auto AIController = Cast< ANPCAIController>(GetController());

		//Montage ���� �� �����̺�� Ʈ�� ����
		if (bDead == true)
			return;
		AIController->PauseLogic();//��͵� ���� ����

	}
	if (bDead == false)
		PlayImpactMontage();//�̰͵� ���� ����
}

bool ANPC_Mob::NetMulticast_DamageImpact_Validate(float Damage)
{
	return true;
}


void ANPC_Mob::PlayImpactMontage()
{

}