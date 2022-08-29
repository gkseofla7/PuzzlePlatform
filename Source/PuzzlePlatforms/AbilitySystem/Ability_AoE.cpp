// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_AoE.h"

#include "../Warrior.h"
#include "../AnimInstance/PlayerAnimInstance.h"

AAbility_AoE::AAbility_AoE()
	:Super()
{
	
}


void AAbility_AoE::BeginPlay()
{
	Super::BeginPlay();

	auto warrior = Cast<AWarrior>(PlayerRef);
	bReplicates = true;
	AsPlayerAnimInstance = Cast<UPlayerAnimInstance>(warrior->GetMesh()->GetAnimInstance());
	//AsPlayerAnimInstance->IsAttacking = true;
	AsPlayerAnimInstance->OnActivateSpell.AddUObject(this, &AAbility_AoE::ActivateEffect);

	AsPlayerAnimInstance->PlayAoEAttackMontage();

}
void AAbility_AoE::CastAbility_Implementation()
{
	Super::CastAbility_Implementation();

}
void AAbility_AoE::ActivateEffect_Implementation()
{
	Super::ActivateEffect_Implementation();
	ParticleSystemComponent->Activate();
	SetActorRotation(FRotator(0, 0, 0).Quaternion());
	const float WeaponRange = 20000.f;
	const FVector StartTrace = PlayerRef->GetActorLocation() ;
	FVector EndTrace = StartTrace + FVector(0,0,-10000);
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(PlayerRef);
	QueryParams.AddIgnoredActors(IgnoreActor);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		SetActorLocation(Hit.Location);

	}
}