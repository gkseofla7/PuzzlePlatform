// Fill out your copyright notice in the Description page of Project Settings.
#include "Warrior.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayersComponent/PlayersMotionReplicator.h"

#include "DrawDebugHelpers.h"

AWarrior::AWarrior()
{
	MotionReplicator = CreateDefaultSubobject<UPlayersMotionReplicator>(TEXT("MOTIOREPLICATOR"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIO_ANIM((TEXT("/Game/Animation/ThirdPerson_AnimBP")));
	if (WARRIO_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIO_ANIM.Class);
	}
}
void AWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);




	MyAnim->OnAttackHitCheck.AddUObject(this, &AWarrior::AttackCheck);

}

void AWarrior::AttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackCheck"));
	if (HasAuthority())
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		float AttackRange = 200.f;
		float AttackRadius = 50.f;


		bool bResult = GetWorld()->SweepSingleByChannel(
			OUT HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * AttackRange,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(AttackRadius),
			Params);
		FVector Vec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + Vec * 0.5f;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
		FColor DrawColor;
		if (bResult)
			DrawColor = FColor::Green;
		else
			DrawColor = FColor::Red;


		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
			Rotation, DrawColor, false, 5.f);
		if (bResult && HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);

		}
	}
}