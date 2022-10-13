// Fill out your copyright notice in the Description page of Project Settings.
#include "Warrior.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayersComponent/WarriorMotionReplicator.h"
#include "Weapons/Sword_Master.h"
#include "MyPlayerController.h"
#include "PuzzlePlatformsGameMode.h"
#include "Sections/RespawnSection.h"
#include "PlayersComponent//MyCharacterStatComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Perception/AISense_Damage.h"

AWarrior::AWarrior()
{
	bReplicates = true;
	ReplicateComponent = CreateDefaultSubobject<UWarriorMotionReplicator>(TEXT("MOTIOREPLICATOR"));

	//static ConstructorHelpers::FClassFinder<ASword_Master> FinderSword(TEXT("/Game/Weapons/BP_Sword_Master"));
	//if (FinderSword.Succeeded())
	//{
	//	SwordClass = FinderSword.Class;
	//	
	//	//EquippedItem->Get
	//}

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIO_ANIM((TEXT("/Game/Animation/BP_WarriorAnimInstance")));
	if (WARRIO_ANIM.Succeeded())
	{
		
		GetMesh()->SetAnimInstanceClass(WARRIO_ANIM.Class);

	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairHudBPClass(TEXT("/Game/Weapons/UI/BP_Crosshair_Hud"));
	if (CrosshairHudBPClass.Succeeded())
	{
		CrosshairHudClass = CrosshairHudBPClass.Class;

	}
	
	TeamNum = 2;

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
	MyAnim = Cast<UAnimInstance_Master>(GetMesh()->GetAnimInstance());

	//EquippedItem = GetWorld()->SpawnActor<ASword_Master>(SwordClass);// GetMesh()->GetSocketTransform("SwordSocket")
	//EquippedItem->GetSkeletalMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//EquippedItem->AttachToPlayer(this, "SwordSocket");
	auto Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	Anim->OnHangMovePlace.AddLambda([this]()->void {
		HangMontageNotify();
		});

	if (CrosshairHudClass != nullptr)
	{

		HudWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairHudClass);
		HudWidget->AddToViewport();

	}

	MyAnim->OnMontageEnded.AddDynamic(this, &AWarrior::EndAnimation);
}
void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ForwardTrace();
	//HeightTrace();
}


void AWarrior::Attack()
{
	//만약 종족이 두개있다면..?
	if (IsAttacking == true || UsingSkill == true)
	{
		if(bNextAttackStart == true)
			bNextAttack = true;
		return;
	}
	SetIsAttacking(true);

	if (ReplicateComponent != nullptr)
		ReplicateComponent->Server_SendAttack();

}

void AWarrior::SetIsAttacking(bool NewIsAttacking)
{
	IsAttacking = NewIsAttacking;
	MyAnim->IsAttacking = NewIsAttacking;
	auto WarriorReplicateComponent = Cast< UWarriorMotionReplicator>(ReplicateComponent);
	if (NewIsAttacking == true)//속도 바꿔주고~
	{
		WarriorReplicateComponent->Server_SetSpeed(300.);//걸을때
	}
	else
	{
		WarriorReplicateComponent->Server_SetSpeed(600.);//뛸때
	}
}

void AWarrior::AttackCheck()
{

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


		//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
//			Rotation, DrawColor, false, 5.f);
		if (bResult && HitResult.Actor.IsValid())
		{
			auto Player = Cast<ACharacter_Master>(HitResult.Actor);
			if (Player != nullptr)
			{
				if (Player->TeamNum == TeamNum)
				{
					return;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("HitCheck"));
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStatRef->AttackDamage, DamageEvent, GetController(), this);
			//UAISense_Damage::ReportDamageEvent(GetWorld(), HitResult.Actor.Get(), this, 10., HitResult.TraceStart, HitResult.Location);
		}
	}
}

void AWarrior::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &AWarrior::ClimbTheWall);
	//PlayerInputComponent->BindAction("Climb", IE_Released, this, &AWarrior::ClimbTheWall);
	PlayerInputComponent->BindAction("ClimbUp", IE_Pressed, this, &AWarrior::ClimbUp);
	PlayerInputComponent->BindAction("DropDown", IE_Pressed, this, &AWarrior::DropDown);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AWarrior::Dash);


}



bool AWarrior::ForwardTrace()
{
	auto StartLoc = GetActorLocation();
	auto EndLoc = StartLoc + GetActorForwardVector() * 150;//Hit, From, To, ECC_Visibility, QueryParams)
	TArray<AActor*> Ignore;
	Ignore.Add(this);
	FHitResult OutHit;
	bool Output = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),StartLoc,EndLoc,10.,ETraceTypeQuery::TraceTypeQuery1,false, Ignore,EDrawDebugTrace::ForOneFrame,OutHit,true);
	WallNormal = OutHit.Normal;
	WallLocation = OutHit.Location;

	return Output;

}

void AWarrior::HeightTrace()
{
	auto StartLoc = GetActorLocation() + FVector(0,0,500.) +  GetActorForwardVector()*75;
	auto EndLoc = StartLoc - FVector(0, 0, 500.);
	TArray<AActor*> Ignore;
	Ignore.Add(this);
	FHitResult OutHit;
	bool ClimbUp = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 10., ETraceTypeQuery::TraceTypeQuery5, false, Ignore, EDrawDebugTrace::ForOneFrame, OutHit, true);
	if (ClimbUp == true)
	{
		HeightLocation = OutHit.Location;
		float tmp = GetMesh()->GetSocketLocation("pelvisSocket").Z - HeightLocation.Z;
		if (tmp >= -50 && tmp <= 0)
		{
			if (IsClimbing == false)
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				GetCharacterMovement()->StopMovementImmediately();
				Hang();
			}
		}
	}

}

void AWarrior::Hang()
{
	auto Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	PlayHangToCrouchMontage();
	Anim->Montage_Pause();
	IsClimbing = true;
	FVector tmpLoc = WallNormal * 20 + WallLocation;
	FVector TargetRelativeLocation = FVector(tmpLoc.X, tmpLoc.Y, HeightLocation.Z - 90);
	FRotator TargetRelativeRotation = UKismetMathLibrary::MakeRotFromX((WallNormal * -1));
	FLatentActionInfo Latentinfo;
	Latentinfo.CallbackTarget = this;
	Latentinfo.ExecutionFunction = "****OnMoveComponentToEnd";
	Latentinfo.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), TargetRelativeLocation, TargetRelativeRotation, true, true, .2, false,EMoveComponentAction::Move, Latentinfo);

}

void AWarrior::PlayHangToCrouchMontage()
{
	auto Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim == nullptr)
	{
		return;
	}

	Cast<UPlayerAnimInstance>(Anim)->PlayHangToCrouchMontage();
}

void AWarrior::ClimbUp()
{
	if (IsClimbing == true)
		Cast<UWarriorMotionReplicator>(ReplicateComponent)->Server_SendClimbUp();
	//if (IsClimbing == true)
	//{
	//	PlayHangToCrouchMontage();
	//}
}
void AWarrior::HangMontageNotify()
{

	FVector TargetRelativeLocation = GetCapsuleComponent()->GetComponentLocation() + 50 * GetActorForwardVector();
	FRotator TargetRelativeRotation = UKismetMathLibrary::MakeRotFromX((GetActorForwardVector()));
	FLatentActionInfo Latentinfo;
	Latentinfo.CallbackTarget = this;
	Latentinfo.ExecutionFunction = "****OnMoveComponentToEnd";
	Latentinfo.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), TargetRelativeLocation, TargetRelativeRotation, true, true, .2, false, EMoveComponentAction::Move, Latentinfo);
	IsClimbing = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}
void AWarrior::DropDown()
{
	if (IsClimbing == true)
	{
		auto Anim = Cast<UPlayerAnimInstance>(MyAnim);
		IsClimbing = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		StopAnimMontage(Anim->GetHangToCrouchMontage());
	}
}

void AWarrior::Dash()
{
	UE_LOG(LogTemp, Warning, TEXT("Dash!!"));
	Cast<UWarriorMotionReplicator>(ReplicateComponent)->Server_SendDash();
}

void AWarrior::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
{
	auto Anim = Cast<UPlayerAnimInstance>(MyAnim);
	if (Montage == Anim->DashMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("DashMontageEnd"));
		IsDashing = false;
		ParticleSystemComponent->SetVisibility(false);
		ParticleSystemComponent->Deactivate();
		GetMesh()->SetVisibility(true);
		EquippedItem->SkeletalMeshComponent->SetVisibility(true);
		Cast<UCharacterMovementComponent>(GetMovementComponent())->GravityScale = 1;
	}
}


void AWarrior::Die()
{
	auto Anim = Cast<UPlayerAnimInstance>(MyAnim);
	if (Anim != nullptr)
	{
		Anim->PlayDeathMontage();
		PlayersDied();
		//FTimerHandle TimerHandler;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASoldier::PlayersDied, 5, false);


	}
}

void AWarrior::PlayersDied()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	//GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	GetCharacterMovement()->DisableMovement();
	FTimerHandle VisibiltyTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(VisibiltyTimerHandler, this, &ACharacter_Master::UnvisiblePlayer, 2.5f, false);
	if (HasAuthority())
	{
		FTimerHandle DestroyTimerHandler;
		EquippedItem->Destroy();
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandler, this, &ACharacter_Master::DestroyPlayer, 10, false);
	}
	if (IsLocallyControlled())
	{
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AWarrior::RespawnCharacter, 5, false);

	}
}

void AWarrior::RespawnCharacter()//Run on Owning Client
{

	Server_RespawnPawn(Cast<APlayerController>(GetController()));
	//Cast<AMyPlayerController>(GetController())->Server_RespawnPawn(GetActorTransform());//위치 임시로
	UnPossessed();
}

void AWarrior::Server_RespawnPawn_Implementation(APlayerController* NewController)
{
	TArray<AActor*>Respawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnSection::StaticClass(), Respawns);
	if (Respawns.Num() == 0)
		return;
	for (auto Respawn : Respawns)
	{
		auto tmp = Cast< ARespawnSection>(Respawn);
		if (tmp->TeamNum == TeamNum)
		{
			auto RespawnTransform = tmp->GetRandomTransform();
			Cast<APuzzlePlatformsGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnRequested(NewController, RespawnTransform, TeamNum);
			break;
		}
	}
	
}

bool AWarrior::Server_RespawnPawn_Validate(APlayerController* NewController)
{
	return true;
}


	//void AWarrior::MoveForward(float Value)
	//{
	//
	//
	//	MoveUpDown = Value;
	//	if ((Value != 0.0f) && IsClimbing == false)
	//	{
	//		// find out which way is forward
	//		const FRotator Rotation = Controller->GetControlRotation();
	//		const FRotator YawRotation(0, Rotation.Yaw, 0);
	//		// get forward vector
	//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//		AddMovementInput(Direction, Value);
	//	}
	//	{
	//		//if (Value == 0)
	//		//	return;
	//
	//		//if (IsClimbing == false)
	//		//{
	//		//	// find out which way is forward
	//		//	const FRotator Rotation = Controller->GetControlRotation();
	//		//	const FRotator YawRotation(0, Rotation.Yaw, 0);
	//		//	// get forward vector
	//		//	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//		//	AddMovementInput(Direction, Value);
	//
	//		//}
	//		//else
	//		//{
	//		//	//if (IsOnEdge == false)
	//		//	//{
	//		//	//	UE_LOG(LogTemp, Warning, TEXT("Wtf"));
	//		//	//	MoveUpDown = 0;
	//		//	//}
	//		//	//else
	//		//	AddMovementInput(GetActorUpVector(), Value);
	//		//}
	//	}
	//}
	//void AWarrior::MoveRight(float Value)
	//{
	//
	//	MoveRightLeft = Value;
	//	{
	//	//if ((Controller != nullptr) && (Value != 0.0f))
	//	//{
	//	//	if (IsClimbing == false)
	//	//	{
	//	//		// find out which way is right
	//	//		const FRotator Rotation = Controller->GetControlRotation();
	//	//		const FRotator YawRotation(0, Rotation.Yaw, 0);
	//
	//	//		// get right vector 
	//	//		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//	//		// add movement in that direction
	//	//		AddMovementInput(Direction, Value);
	//	//	}
	//	//	else
	//	//	{
	//	//		AddMovementInput(GetActorRightVector(), Value);
	//	//	}
	//	//}
	//	}
	//
	//
	//		if ((Controller != nullptr) && (Value != 0.0f)&&IsClimbing==false)
	//		{
	//
	//			AddMovementInput(GetActorRightVector(), Value);
	//		}
	//}
	//void AWarrior::ClimbTheWall()
	//{
	//	if(ClimbTheWallOn == false)
	//		ClimbTheWallOn = true;
	//	else
	//	{
	//		ClimbTheWallOn = false;
	//	}
	//	if (IsClimbing)
	//	{
	//		FVector tmp = GetActorForwardVector() * -500;
	//		LaunchCharacter(FVector(tmp.X, tmp.Y, 0), false, false);
	//		JumpFromWall();
	//	}
	//
	//}
	//void AWarrior::Climb()
	//{
	//	bool CanClimb = ForwardTrace();
	//	if (CanClimb&&IsClimbing ==false)
	//	{
	//		
	//		float YawValue = UKismetMathLibrary::MakeRotFromX(WallNormal).Yaw + 180;
	//		SetActorRotation(FRotator(0, YawValue, 0));
	//		IsClimbing = true;
	//		GetCharacterMovement()->Velocity =(FVector(0, 0, 0));
	//		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	//	}
	//}
