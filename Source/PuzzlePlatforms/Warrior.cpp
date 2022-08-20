// Fill out your copyright notice in the Description page of Project Settings.
#include "Warrior.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayersComponent/PlayersMotionReplicator.h"
#include "Weapons/Sword_Master.h"

#include "DrawDebugHelpers.h"

AWarrior::AWarrior()
{
	DaerimMotionReplicator = CreateDefaultSubobject<UPlayersMotionReplicator>(TEXT("MOTIOREPLICATOR"));

	static ConstructorHelpers::FClassFinder<ASword_Master> FinderSword(TEXT("/Game/Weapons/BP_Sword_Master"));
	if (FinderSword.Succeeded())
	{
		SwordClass = FinderSword.Class;
		
		//EquippedItem->Get
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIO_ANIM((TEXT("/Game/Animation/ThirdPerson_AnimBP")));
	if (WARRIO_ANIM.Succeeded())
	{
		
		GetMesh()->SetAnimInstanceClass(WARRIO_ANIM.Class);

	}
}
void AWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != Anim);




	Anim->OnAttackHitCheck.AddUObject(this, &AWarrior::AttackCheck);

}
void AWarrior::BeginPlay()
{
	Super::BeginPlay();

	EquippedItem = GetWorld()->SpawnActor<ASword_Master>(SwordClass);// GetMesh()->GetSocketTransform("SwordSocket")
	EquippedItem->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedItem->AttachToPlayer(this, "SwordSocket");
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

void AWarrior::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &AWarrior::ClimbTheWall);
	PlayerInputComponent->BindAction("Climb", IE_Released, this, &AWarrior::ClimbTheWall);

}


void AWarrior::MoveForward(float Value)
{
	MoveUpDown = Value;
	if ((Value != 0.0f) && IsClimbing == false)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	//if ((Controller != nullptr) && (Value != 0.0f))
	//{

	//	if (IsClimbing == false)
	//	{


	//		
	//		// find out which way is forward
	//		const FRotator Rotation = Controller->GetControlRotation();
	//		const FRotator YawRotation(0, Rotation.Yaw, 0);

	//		// get forward vector
	//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//		AddMovementInput(Direction, Value);
	//		
	//	}
	//	else
	//	{
	//		if (IsOnEdge == false)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Wtf"));
	//			MoveUpDown = 0;
	//		}
	//		else
	//			AddMovementInput(GetActorUpVector(), Value);
	//	}
	//}


}

void AWarrior::MoveRight(float Value)
{
	MoveRightLeft = Value;
	//if ((Controller != nullptr) && (Value != 0.0f))
	//{
	//	if (IsClimbing == false)
	//	{
	//		// find out which way is right
	//		const FRotator Rotation = Controller->GetControlRotation();
	//		const FRotator YawRotation(0, Rotation.Yaw, 0);

	//		// get right vector 
	//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//		// add movement in that direction
	//		AddMovementInput(Direction, Value);
	//	}
	//	else
	//	{
	//		AddMovementInput(GetActorRightVector(), Value);
	//	}
	//}

		if ((Controller != nullptr) && (Value != 0.0f)&&IsClimbing==false)
		{

			AddMovementInput(GetActorRightVector(), Value);
		}
}


void AWarrior::ClimbTheWall()
{
	if(ClimbTheWallOn == false)
		ClimbTheWallOn = true;
	else
	{
		ClimbTheWallOn = false;
	}
	if (IsClimbing)
	{
		FVector tmp = GetActorForwardVector() * -500;
		LaunchCharacter(FVector(tmp.X, tmp.Y, 0), false, false);
		JumpFromWall();
	}

}
//void AWarrior::CJump(float Value)
//{
//	if (IsClimbing)
//	{
//		FVector tmp = GetActorForwardVector() * -500;
//		LaunchCharacter(FVector(tmp.X, tmp.Y, 0), false, false);
//		JumpFromWall();
//
//	}
//	else
//	{
//		Jump();
//	}
//}

