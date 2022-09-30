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

	PrimaryComponentTick.bCanEverTick = true;

}

//void USoldierMotionReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(USoldierMotionReplicator, PickupItem);
//	//DOREPLIFETIME(USoldierMotionReplicator, IsFiring);
//
//}

void USoldierMotionReplicator::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	auto Character = Cast<ACharacter>(GetOwner());
	MyAnim = Cast<USoldierAnimInstance>(Character->GetMesh()->GetAnimInstance());
}


void USoldierMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void USoldierMotionReplicator::Server_SetMuzzleRotation_Implementation(FRotator NewRotator)
{
	NetMulticast_SetMuzzleRotation(NewRotator);
}
bool USoldierMotionReplicator::Server_SetMuzzleRotation_Validate(FRotator NewRotator)
{
	return true;
}

void USoldierMotionReplicator::NetMulticast_SetMuzzleRotation_Implementation(FRotator NewRotator)
{
	auto Soldier = Cast<ASoldier>(GetOwner());
	if (Soldier->EquippedItem == nullptr)
		return;
	Soldier->EquippedItem->SetMuzzleRotation(NewRotator);

}
bool USoldierMotionReplicator::NetMulticast_SetMuzzleRotation_Validate(FRotator NewRotator)
{
	return true;
}


void USoldierMotionReplicator::Server_WeaponReload_Implementation()
{
	NetMulticast_WeaponReload();
}
bool USoldierMotionReplicator::Server_WeaponReload_Validate()
{
	return true;
}

void USoldierMotionReplicator::NetMulticast_WeaponReload_Implementation()
{
	auto Soldier = Cast<ASoldier>(GetOwner());
	Soldier->CanAim = false;
	Soldier->IsReloading = true;

	FTimerHandle WaitHandle;
	Soldier->EquippedItem->Reload();
	float WaitTime = Soldier->EquippedItem->ReloadDelay; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto Soldier = Cast<ASoldier>(GetOwner());
			Soldier->IsReloading = false;
			Soldier->CanAim = true;
		}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
}

bool USoldierMotionReplicator::NetMulticast_WeaponReload_Validate()
{
	return true;
}


void USoldierMotionReplicator::Server_SendAttack_Implementation()
{
	auto MyOwner = Cast<ASoldier>(GetOwner());
	if (MyOwner&&MyOwner->EquippedItem == nullptr)
	{
		return;
	}
	MyOwner->EquippedItem->StartFire();

	IsFiring = true;

}

void USoldierMotionReplicator::Server_SendAttackStop_Implementation()
{
	auto MyOwner = Cast<ASoldier>(GetOwner());

	if (MyOwner&&MyOwner->EquippedItem == nullptr)
	{
		return;
	}
	MyOwner->EquippedItem->StopFire();
	IsFiring = false;
}



void USoldierMotionReplicator::Server_SendGetItem_Implementation(class AObject_Master* NewWeapon)
{
	Multicast_SendGetItem(NewWeapon);

}


void USoldierMotionReplicator::Multicast_SendGetItem_Implementation(class AObject_Master* NewWeapon)
{

	auto MyOwner = Cast<ASoldier>(GetOwner());

	NewWeapon->SetOwner(MyOwner);
	MyOwner->EquipItem(NewWeapon, MyOwner->IsItemEquipped);

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



bool USoldierMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

bool USoldierMotionReplicator::Server_SendAttackStop_Validate()
{
	return true;
}

bool USoldierMotionReplicator::Server_SendGetItem_Validate(class AObject_Master* NewWeapon)
{
	return true;
}
bool USoldierMotionReplicator::Multicast_SendGetItem_Validate(class AObject_Master* NewWeapon)
{
	return true;
}


//void USoldierMotionReplicator::Server_SendRide_Implementation(AActor* _Car, APawn* _Rider)
//{
//	auto Car = Cast<AMyProjectPawn>(_Car);
//
//	//auto Rider = Cast<ACharacter_Master>(this);
//
//	UE_LOG(LogTemp, Warning, TEXT("Ride"));
//	auto Rider = Cast<APawn>(GetOwner());
//	if (Car != nullptr)
//	{
//		//Car->SetRider(this);
//		DisableActor(true);
//		Car->AIController = Car->GetController();
//		Rider->GetController()->Possess(Car);
//		Car->SetRider(Rider);
//
//	}
//}
//bool USoldierMotionReplicator::Server_SendRide_Validate(AActor* Car, APawn* Rider)
//{
//	return true;
//
//}