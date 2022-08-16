// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierMotionReplicator.h"

#include "../Cars/MyProjectPawn.h"
#include "../AnimInstance/SoldierAnimInstance.h"
#include "../Soldier.h"
#include "../Weapons/Weapon_Master.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USoldierMotionReplicator::USoldierMotionReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USoldierMotionReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USoldierMotionReplicator, IsFiring);
	DOREPLIFETIME(USoldierMotionReplicator, PickupItem);

}
// Called when the game starts
void USoldierMotionReplicator::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	auto Character = Cast<ACharacter>(GetOwner());
	MyAnim = Cast<USoldierAnimInstance>(Character->GetMesh()->GetAnimInstance());
	// ...
	
}


// Called every frame
void USoldierMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





void USoldierMotionReplicator::Server_SendRide_Implementation(AActor* _Car, APawn* _Rider)
{
	auto Car = Cast<AMyProjectPawn>(_Car);

	//auto Rider = Cast<APuzzlePlatformsCharacter>(this);

	UE_LOG(LogTemp, Warning, TEXT("Ride"));
	auto Rider = Cast<APawn>(GetOwner());
	if (Car != nullptr)
	{
		//Car->SetRider(this);
		DisableActor(true);
		Car->AIController = Car->GetController();
		Rider->GetController()->Possess(Car);
		Car->SetRider(Rider);

	}
}
bool USoldierMotionReplicator::Server_SendRide_Validate(AActor* Car, APawn* Rider)
{
	return true;

}

void USoldierMotionReplicator::Server_SendAttack_Implementation()
{

	auto MyOwner = Cast<ASoldier>(GetOwner());
	//MyOwner ->SetMuzzleRotation();
	MyOwner->EquippedItem->StartFire();
	UE_LOG(LogTemp, Warning, TEXT("False!!"));
	IsFiring = true;
	

}

bool USoldierMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

void USoldierMotionReplicator::Server_SendAttackStop_Implementation()
{
	auto MyOwner = Cast<ASoldier>(GetOwner());
	MyOwner->EquippedItem->StopFire();
	IsFiring = false;
}

bool USoldierMotionReplicator::Server_SendAttackStop_Validate()
{
	return true;
}


void USoldierMotionReplicator::Server_SendGetItem_Implementation(class AObject_Master* NewWeapon)
{
	PickupItem = NewWeapon;

	if (PickupItem == nullptr)
		return;

	Multicast_SendGetItem();

}

bool USoldierMotionReplicator::Server_SendGetItem_Validate(class AObject_Master* NewWeapon)
{
	return true;
}

void USoldierMotionReplicator::Multicast_SendGetItem_Implementation()
{

	auto MyOwner = Cast<ASoldier>(GetOwner());
	if (MyOwner->HasAuthority())
		return;
	if (MyOwner->DoPickupLinetrace)
	{
		if (PickupItem != nullptr)
		{

			MyOwner->EquipItem(PickupItem, MyOwner->IsItemEquipped);
			//FTimerHandle WaitHandle;
			//float WaitTime = 1.73; //시간을 설정하고
			MyOwner->CanAim = true;//나중에 애니메이션 노티파이로변환

		}
	}
}

bool USoldierMotionReplicator::Multicast_SendGetItem_Validate()
{
	return true;
}


void USoldierMotionReplicator::DisableActor(bool toHide)
{
	// Hides visible components
	GetOwner()->SetActorHiddenInGame(toHide);

	// Disables collision components
	GetOwner()->SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	GetOwner()->SetActorTickEnabled(!toHide);
}

void USoldierMotionReplicator::OnRep_Attack()
{
	auto MyOwner = Cast<ASoldier>(GetOwner());
	if (IsFiring == true)
	{
		
		MyOwner->EquippedItem->StartFire();

	}
	else
	{
		MyOwner->EquippedItem->StopFire();
	}
}