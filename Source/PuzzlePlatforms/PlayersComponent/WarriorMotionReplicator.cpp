// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorMotionReplicator.h"

#include "../AnimInstance/PlayerAnimInstance.h"
#include "../Cars/GoKart.h"
#include "../Warrior.h"
#include "../Weapons/Sword_Master.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWarriorMotionReplicator::UWarriorMotionReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxCombo = 3;
	CurrentCombo = 0;

	//AttackEndComboState();
	bWantsInitializeComponent = true;

	// ...
}

void UWarriorMotionReplicator::InitializeComponent()
{

 }


void UWarriorMotionReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWarriorMotionReplicator, AttackToggle);
	DOREPLIFETIME(UWarriorMotionReplicator, CurrentCombo);


}


// Called when the game starts
void UWarriorMotionReplicator::BeginPlay()
{
	Super::BeginPlay();
	

	SetIsReplicated(true);
	// ...
	auto Character = Cast<ACharacter>(GetOwner());
	MyAnim = Cast<UPlayerAnimInstance>(Character->GetMesh()->GetAnimInstance());


	MyAnim->OnAttackEndCheck.AddLambda([this]()->void {
		OnAttackMontageEnded();
		});


}


// Called every frame
void UWarriorMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





void UWarriorMotionReplicator::Server_SendAttack_Implementation()
{

	if (MyAnim->IsAttacking == false)
	{

		CurrentCombo++;
		if (CurrentCombo == 4)
			CurrentCombo = 1;
		MyAnim->IsAttacking = true;
		PlaySwordAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		AttackToggle = !AttackToggle;

	}

}

bool UWarriorMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

void UWarriorMotionReplicator::Server_SendClimbUp_Implementation()
{

	//auto Warrior = Cast<AWarrior>(GetOwner());
	//if (Warrior->IsClimbing == true)
	//{
	//	Warrior->PlayHangToCrouchMontage();
	//}
	NetMulticast_SendClimbUp();
}


void UWarriorMotionReplicator::Server_SendDash_Implementation()
{
	NetMulticast_SendDash();
}

void UWarriorMotionReplicator::NetMulticast_SendDash_Implementation()
{
	auto Warrior = Cast<AWarrior>(GetOwner());
	Warrior->IsDashing = true;
	MyAnim->PlayDashMontage();
	Warrior->ParticleSystemComponent->SetVisibility(true);
	Warrior->ParticleSystemComponent->Activate(true);
	Warrior->GetMesh()->SetVisibility(false);
	Warrior->EquippedItem->SkeletalMeshComponent->SetVisibility(false);
	Cast<UCharacterMovementComponent>(Warrior->GetMovementComponent())->GravityScale = 0;

}

bool UWarriorMotionReplicator::Server_SendDash_Validate()
{
	return true;
}

bool UWarriorMotionReplicator::NetMulticast_SendDash_Validate()
{
	return true;
}

bool UWarriorMotionReplicator::Server_SendClimbUp_Validate()
{
	return true;
}
void UWarriorMotionReplicator::NetMulticast_SendClimbUp_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Climb"), *GetOwner()->GetName());
	auto Warrior = Cast<AWarrior>(GetOwner());
	if (Warrior->IsClimbing == true)
	{
		Warrior->PlayHangToCrouchMontage();
	}
}

bool UWarriorMotionReplicator::NetMulticast_SendClimbUp_Validate()
{
	return true;
}

void UWarriorMotionReplicator::PlaySwordAttackMontage()
{
	if (MyAnim == nullptr)
	{
		return;
	}
	MyAnim->PlaySwordAttackMontage();
}

void UWarriorMotionReplicator::DisableActor(bool toHide)
{
	// Hides visible components
	GetOwner()->SetActorHiddenInGame(toHide);

	// Disables collision components
	GetOwner()->SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	GetOwner()->SetActorTickEnabled(!toHide);
}

void UWarriorMotionReplicator::OnRep_Attack()//서버에서는 안하는구나..?ㅋ
{
	if (MyAnim == nullptr)
	{
		//MyAnim->IsAttacking = false;
		return;
	}
	MyAnim->IsAttacking = true;
	PlaySwordAttackMontage();
	MyAnim->JumpToAttackMontageSection(CurrentCombo);
}



void UWarriorMotionReplicator::OnAttackMontageEnded()
{
	if (MyAnim == nullptr)
	{
		//MyAnim->IsAttacking = false;
		return;
	}
	MyAnim->IsAttacking = false;

}


//void UWarriorMotionReplicator::Server_SendRide_Implementation(AActor* _Car, APawn* _Rider)
//{
//
//
//	auto Car = Cast<AGoKart>(_Car);
//	auto Rider = Cast<APawn>(GetOwner());
//	if (Car != nullptr)
//	{
//
//		//Car->SetRider(this);
//
//		DisableActor(true);
//
//		Car->AIController = Car->GetController();
//		Rider->GetController()->Possess(Car);
//		Car->SetRider(Rider);
//
//	}
//}
//
//bool UWarriorMotionReplicator::Server_SendRide_Validate(AActor* Car, APawn* Rider)
//{
//	return true;
//
//}