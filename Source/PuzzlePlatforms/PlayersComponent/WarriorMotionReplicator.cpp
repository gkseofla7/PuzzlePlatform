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

	MyAnim->OnMontageEnded.AddDynamic(this, &UWarriorMotionReplicator::OnAttackMontageEnded);
	MyAnim->OnNextAttackCheck.AddUObject(this, &UWarriorMotionReplicator::NextAttack);
	
	WarriorRef = Cast<AWarrior>(Character);
}


// Called every frame
void UWarriorMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UWarriorMotionReplicator::NextAttack()
{
	WarriorRef->bNextAttackStart = true;
}


void UWarriorMotionReplicator::Server_SendAttack_Implementation()
{
	NetMulticast_SendAttack();
}
bool UWarriorMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

void UWarriorMotionReplicator::NetMulticast_SendAttack_Implementation()
{
	//if (MyAnim->IsAttacking == false)
//{



//}
	CurrentCombo++;
	if (CurrentCombo == 4)
		CurrentCombo = 1;
	//MyAnim->IsAttacking = true;
	PlaySwordAttackMontage();
	MyAnim->JumpToAttackMontageSection(CurrentCombo);



}

bool UWarriorMotionReplicator::NetMulticast_SendAttack_Validate()
{
	return true;
}

void UWarriorMotionReplicator::Server_SetSpeed_Implementation(float NewSpeed)
{
	WarriorRef->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

}

bool UWarriorMotionReplicator::Server_SetSpeed_Validate(float NewSpeed)
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
	WarriorRef->IsDashing = true;
	MyAnim->PlayDashMontage();
	WarriorRef->ParticleSystemComponent->SetVisibility(true);
	WarriorRef->ParticleSystemComponent->Activate(true);
	WarriorRef->GetMesh()->SetVisibility(false);
	//WarriorRef->EquippedItem->SkeletalMeshComponent->SetVisibility(false);
	Cast<UCharacterMovementComponent>(WarriorRef->GetMovementComponent())->GravityScale = 0;

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
	if (WarriorRef->IsClimbing == true)
	{
		WarriorRef->PlayHangToCrouchMontage();
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



void UWarriorMotionReplicator::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (MyAnim == nullptr)
	{
		return;
	}
	if (Montage != MyAnim->SwordAttackMontage)
		return;


	WarriorRef->bNextAttackStart = false;
	if (WarriorRef->bNextAttack == true)
	{
		WarriorRef->bNextAttack = false;
		Server_SendAttack();

	}
	else
	{
		WarriorRef->SetIsAttacking(false);
	}
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