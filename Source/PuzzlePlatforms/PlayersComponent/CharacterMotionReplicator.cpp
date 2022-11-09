// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMotionReplicator.h"

#include "../AbilitySystem/Ability.h"
#include "../Character_Master.h"
#include "../AnimInstance/AnimInstance_Master.h"
// Sets default values for this component's properties
UCharacterMotionReplicator::UCharacterMotionReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterMotionReplicator::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	PlayerRef = Cast<ACharacter_Master>(GetOwner());
	// ...
	
}

void UCharacterMotionReplicator::Server_SetMaxWalkSpeed_Implementation(float NewWalkSpeed)
{
	NetMulticast_SetMaxWalkSpeed(NewWalkSpeed);
}

bool UCharacterMotionReplicator::Server_SetMaxWalkSpeed_Validate(float NewWalkSpeed)
{
	return true;
}


void  UCharacterMotionReplicator::NetMulticast_SetMaxWalkSpeed_Implementation(float NewWalkSpeed)
{
	PlayerRef->GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
}

bool  UCharacterMotionReplicator::NetMulticast_SetMaxWalkSpeed_Validate(float NewWalkSpeed)
{
	return true;
}

void UCharacterMotionReplicator::Server_BindCharacterStatToWidget_Implementation()
{
	NetMulticast_BindCharacterStatToWidget();
}

bool UCharacterMotionReplicator::Server_BindCharacterStatToWidget_Validate()
{
	return true;
}
void  UCharacterMotionReplicator::NetMulticast_BindCharacterStatToWidget_Implementation()
{
	
	PlayerRef->BindCharacterStatToWidget();
}

bool  UCharacterMotionReplicator::NetMulticast_BindCharacterStatToWidget_Validate()
{
	return true;
}


void UCharacterMotionReplicator::Server_SendRide(AActor* _Car, APawn* _Rider)
{

}

void UCharacterMotionReplicator::Server_SendAttack()
{

}
void UCharacterMotionReplicator::DisableActor(bool toHide)
{

}


void UCharacterMotionReplicator::Server_Skill1Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
}

void UCharacterMotionReplicator::Server_Skill2Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
}


void UCharacterMotionReplicator::Server_Skill3Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
}

void UCharacterMotionReplicator::Server_Skill4Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);

}
void UCharacterMotionReplicator::Server_Skill5Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);

}

bool UCharacterMotionReplicator::Server_Skill1Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
bool UCharacterMotionReplicator::Server_Skill2Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
bool UCharacterMotionReplicator::Server_Skill3Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}

bool UCharacterMotionReplicator::Server_Skill4Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}

bool UCharacterMotionReplicator::Server_Skill5Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
void UCharacterMotionReplicator::AbilitySpawn(TSubclassOf<AAbility>AbilityClass)
{
	FTransform PlayerTransform = GetOwner()->GetActorTransform();
	FActorSpawnParameters Params;
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner = GetOwner();
	SpawnInfo.Instigator = Cast<APawn>(GetOwner());
	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform, SpawnInfo);
}



void UCharacterMotionReplicator::Server_SetTargetPlayer_Implementation(ACharacter_Master* NewTarget)
{
	NetMulticast_SetTargetPlayer(NewTarget);
}

void UCharacterMotionReplicator::NetMulticast_SetTargetPlayer_Implementation(ACharacter_Master* NewTarget)
{

	PlayerRef->TargetPlayer = NewTarget;
}

bool UCharacterMotionReplicator::Server_SetTargetPlayer_Validate(ACharacter_Master* NewTarget)
{
	return true;
}

bool UCharacterMotionReplicator::NetMulticast_SetTargetPlayer_Validate(ACharacter_Master* NewTarget)
{
	return true;
}




void UCharacterMotionReplicator::Server_SetbUsingSkill_Implementation(bool NewbUsingSkill)
{
	NetMulticast_SetbUsingSkill(NewbUsingSkill);
}

void UCharacterMotionReplicator::NetMulticast_SetbUsingSkill_Implementation(bool NewbUsingSkill)
{
	PlayerRef->bUsingSkill = NewbUsingSkill;
	PlayerRef->MyAnim->bUsingSkill = NewbUsingSkill;
}

bool UCharacterMotionReplicator::Server_SetbUsingSkill_Validate(bool NewbUsingSkill)
{
	return true;
}

bool UCharacterMotionReplicator::NetMulticast_SetbUsingSkill_Validate(bool NewbUsingSkill)
{
	return true;
}


void UCharacterMotionReplicator::Server_SetbIsAttacking_Implementation(bool NewbIsAttacking)
{
	NetMulticast_SetbIsAttacking(NewbIsAttacking);
}

void UCharacterMotionReplicator::NetMulticast_SetbIsAttacking_Implementation(bool NewbIsAttacking)
{
	PlayerRef->bIsAttacking = NewbIsAttacking;
	PlayerRef->MyAnim->bIsAttacking = NewbIsAttacking;
}
bool UCharacterMotionReplicator::Server_SetbIsAttacking_Validate(bool NewbIsAttacking)
{
	return true;
}

bool UCharacterMotionReplicator::NetMulticast_SetbIsAttacking_Validate(bool NewbIsAttacking)
{
	return true;
}