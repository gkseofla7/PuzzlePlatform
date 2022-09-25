// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionInterfaceComponent.h"

#include "../AbilitySystem/Ability.h"
#include "../Character_Master.h"
#include "../AnimInstance/AnimInstance_Master.h"
// Sets default values for this component's properties
UMotionInterfaceComponent::UMotionInterfaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	PlayerRef = Cast<ACharacter_Master>(GetOwner());
	// ...
	
}


// Called every frame
void UMotionInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UMotionInterfaceComponent::Server_SendRide(AActor* _Car, APawn* _Rider)
{

}

void UMotionInterfaceComponent::Server_SendAttack()
{

}
void UMotionInterfaceComponent::DisableActor(bool toHide)
{

}


void UMotionInterfaceComponent::Server_Skill1Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
	//NewHeadsUpDisplay->ActionBar_UI->ActionBarSlot_UI->AbilitySpawn(this);

}

void UMotionInterfaceComponent::Server_Skill2Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
}


void UMotionInterfaceComponent::Server_Skill3Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);
}

void UMotionInterfaceComponent::Server_Skill4Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);

}
void UMotionInterfaceComponent::Server_Skill5Clicked_Implementation(TSubclassOf<AAbility>AbilityClass)
{
	AbilitySpawn(AbilityClass);

}

bool UMotionInterfaceComponent::Server_Skill1Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
bool UMotionInterfaceComponent::Server_Skill2Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
bool UMotionInterfaceComponent::Server_Skill3Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}

bool UMotionInterfaceComponent::Server_Skill4Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}

bool UMotionInterfaceComponent::Server_Skill5Clicked_Validate(TSubclassOf<AAbility>AbilityClass)
{
	return true;
}
void UMotionInterfaceComponent::AbilitySpawn(TSubclassOf<AAbility>AbilityClass)
{

	UE_LOG(LogTemp, Warning, TEXT("Spawn In Server"));
	FTransform PlayerTransform = GetOwner()->GetActorTransform();
	FActorSpawnParameters Params;
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner = GetOwner();
	SpawnInfo.Instigator = Cast<APawn>(GetOwner());
	//auto ability = GetWorld()->SpawnActorDeferred<AAbility>(AbilityClass, PlayerTransform,NewPlayer,NewPlayer);
	auto ability = GetWorld()->SpawnActor<AAbility>(AbilityClass, PlayerTransform, SpawnInfo);

	//ability->PlayerRef = NewPlayer;//컨트롤러가 있는 플레이어
//UWorld::SpawnActor(AbilityClass, PlayerTransform);
//GetWorld()->SpawnActor();
}



void UMotionInterfaceComponent::Server_SetTargetPlayer_Implementation(ACharacter_Master* NewTarget)
{
	NetMulticast_SetTargetPlayer(NewTarget);
}

void UMotionInterfaceComponent::NetMulticast_SetTargetPlayer_Implementation(ACharacter_Master* NewTarget)
{

	PlayerRef->TargetPlayer = NewTarget;
}

bool UMotionInterfaceComponent::Server_SetTargetPlayer_Validate(ACharacter_Master* NewTarget)
{
	return true;
}

bool UMotionInterfaceComponent::NetMulticast_SetTargetPlayer_Validate(ACharacter_Master* NewTarget)
{
	return true;
}

void UMotionInterfaceComponent::Server_SetIsAttacking_Implementation(bool NewIsAttacking)
{
	NetMulticast_SetIsAttacking(NewIsAttacking);
}

void UMotionInterfaceComponent::NetMulticast_SetIsAttacking_Implementation(bool NewIsAttacking)
{
	PlayerRef->IsAttacking = NewIsAttacking;
	PlayerRef->MyAnim->IsAttacking = NewIsAttacking;
}
bool UMotionInterfaceComponent::Server_SetIsAttacking_Validate(bool NewIsAttacking)
{
	return true;
}

bool UMotionInterfaceComponent::NetMulticast_SetIsAttacking_Validate(bool NewIsAttacking)
{
	return true;
}


void UMotionInterfaceComponent::Server_SetUsingSkill_Implementation(bool NewUsingSkill)
{
	NetMulticast_SetUsingSkill(NewUsingSkill);
}

void UMotionInterfaceComponent::NetMulticast_SetUsingSkill_Implementation(bool NewUsingSkill)
{
	PlayerRef->UsingSkill = NewUsingSkill;
	PlayerRef->MyAnim->UsingSkill = NewUsingSkill;
}

bool UMotionInterfaceComponent::Server_SetUsingSkill_Validate(bool NewUsingSkill)
{
	return true;
}

bool UMotionInterfaceComponent::NetMulticast_SetUsingSkill_Validate(bool NewUsingSkill)
{
	return true;
}