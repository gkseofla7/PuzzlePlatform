// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierMotionReplicator.h"

#include "../Cars/GoKart.h"
#include "../AnimInstance/SoldierAnimInstance.h"

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


// Called when the game starts
void USoldierMotionReplicator::BeginPlay()
{
	Super::BeginPlay();
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
	auto Car = Cast<AGoKart>(_Car);

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
	//if (MyAnim->IsAttacking == false)
	//{
	//	//MyAnim->IsAttacking = true;
	//	//PlaySwordAttackMontage();
	//	//AttackToggle = !AttackToggle;

	//}

}

bool USoldierMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

//void USoldierMotionReplicator::PlaySwordAttackMontage()
//{
//	if (MyAnim == nullptr)
//	{
//		//MyAnim->IsAttacking = false;
//		return;
//	}
//	//MyAnim->PlaySwordAttackMontage();
//}

void USoldierMotionReplicator::DisableActor(bool toHide)
{
	// Hides visible components
	GetOwner()->SetActorHiddenInGame(toHide);

	// Disables collision components
	GetOwner()->SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	GetOwner()->SetActorTickEnabled(!toHide);
}