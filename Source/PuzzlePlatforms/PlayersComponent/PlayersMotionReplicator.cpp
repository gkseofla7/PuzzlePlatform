// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayersMotionReplicator.h"

#include "../AnimInstance/PlayerAnimInstance.h"
#include "../Cars/GoKart.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayersMotionReplicator::UPlayersMotionReplicator()
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

void UPlayersMotionReplicator::InitializeComponent()
{

 }


void UPlayersMotionReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayersMotionReplicator, AttackToggle);
	DOREPLIFETIME(UPlayersMotionReplicator, CurrentCombo);


}


// Called when the game starts
void UPlayersMotionReplicator::BeginPlay()
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
void UPlayersMotionReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UPlayersMotionReplicator::Server_SendRide_Implementation(AActor* _Car, APawn* _Rider)
{


	auto Car = Cast<AGoKart>(_Car);
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

bool UPlayersMotionReplicator::Server_SendRide_Validate(AActor* Car, APawn* Rider)
{
	return true;

}

void UPlayersMotionReplicator::Server_SendAttack_Implementation()
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

bool UPlayersMotionReplicator::Server_SendAttack_Validate()
{
	return true;
}

void UPlayersMotionReplicator::PlaySwordAttackMontage()
{
	if (MyAnim == nullptr)
	{
		return;
	}
	MyAnim->PlaySwordAttackMontage();
}

void UPlayersMotionReplicator::DisableActor(bool toHide)
{
	// Hides visible components
	GetOwner()->SetActorHiddenInGame(toHide);

	// Disables collision components
	GetOwner()->SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	GetOwner()->SetActorTickEnabled(!toHide);
}

void UPlayersMotionReplicator::OnRep_Attack()//서버에서는 안하는구나..?ㅋ
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



void UPlayersMotionReplicator::OnAttackMontageEnded()
{
	if (MyAnim == nullptr)
	{
		//MyAnim->IsAttacking = false;
		return;
	}
	MyAnim->IsAttacking = false;

}