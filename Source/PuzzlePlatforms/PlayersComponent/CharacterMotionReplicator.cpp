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


// Called every frame
void UCharacterMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

void UCharacterMotionReplicator::Server_SetIsAttacking_Implementation(bool NewIsAttacking)
{
	NetMulticast_SetIsAttacking(NewIsAttacking);
}

void UCharacterMotionReplicator::NetMulticast_SetIsAttacking_Implementation(bool NewIsAttacking)
{
	PlayerRef->IsAttacking = NewIsAttacking;
	PlayerRef->MyAnim->IsAttacking = NewIsAttacking;
}
bool UCharacterMotionReplicator::Server_SetIsAttacking_Validate(bool NewIsAttacking)
{
	return true;
}

bool UCharacterMotionReplicator::NetMulticast_SetIsAttacking_Validate(bool NewIsAttacking)
{
	return true;
}


void UCharacterMotionReplicator::Server_SetUsingSkill_Implementation(bool NewUsingSkill)
{
	NetMulticast_SetUsingSkill(NewUsingSkill);
}

void UCharacterMotionReplicator::NetMulticast_SetUsingSkill_Implementation(bool NewUsingSkill)
{
	PlayerRef->UsingSkill = NewUsingSkill;
	PlayerRef->MyAnim->UsingSkill = NewUsingSkill;
}

bool UCharacterMotionReplicator::Server_SetUsingSkill_Validate(bool NewUsingSkill)
{
	return true;
}

bool UCharacterMotionReplicator::NetMulticast_SetUsingSkill_Validate(bool NewUsingSkill)
{
	return true;
}