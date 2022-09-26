// Fill out your copyright notice in the Description page of Project Settings.

#include "../PuzzlePlatforms.h"
#include "NPC_Master.h"
#include "NPCAIController.h"
#include "GoblinAnimInstance.h"
#include "MonsterStatComponent.h"
#include "../UI/HPBarWidget.h"
#include "../Character_Master.h"
#include "NPC_Archer.h"
#include "../MyPlayerState.h"
#include "../PlayersComponent/MyCharacterStatComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANPC_Master::ANPC_Master()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//static ConstructorHelpers::FClassFinder<ANPCAIController> AIControllerBPClass(TEXT("/Game/AI/NPC/NPC_AI_Controller"));
	//if (AIControllerBPClass.Class != NULL)
	//{
	//	AIControllerClass = AIControllerBPClass.Class;
	//}
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	MonsterStat = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStat"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/PuzzlePlatforms/Widget/WBP_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}
}

// Called when the game starts or when spawned
void ANPC_Master::BeginPlay()
{
	Super::BeginPlay();

	auto MonsterWidget = Cast< UHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != MonsterWidget)
	{
		MonsterWidget->BindMonsterStat(MonsterStat);
	}

}

// Called every frame
void ANPC_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABCHECK(PlayerController != nullptr);
	auto Pawn = PlayerController->GetPawn();
	if (Pawn == nullptr)
		return;
	auto Dir = PlayerController->GetPawn()->GetActorLocation() - GetActorLocation();
	auto DirRot = UKismetMathLibrary::MakeRotFromX(Dir);
	HPBarWidget->SetWorldRotation(DirRot);

	

}

// Called to bind functionality to input
void ANPC_Master::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Master::Attack()
{
	//NetMulticast_Attack();
}






//void ANPC_Master::EndAnimation(UAnimMontage* Montage, bool bInterrupted)
//{
//
//
//}


float ANPC_Master::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead == true)
		return 0;
	if (!HasAuthority())
		return 0;

	//ABCHECK(MotionReplicator != nullptr)

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AttackedPlayer = DamageCauser;
	auto Player = Cast< ACharacter_Master>(DamageCauser);
	Cast<ANPCAIController>(GetController())->SetTargetKey(Player);
	Cast<ANPCAIController>(GetController())->SetIsHitKey(true);
	auto Archer = Cast<ANPC_Archer>(this);
	if (Archer != nullptr)
	{
		Archer->NetMulticast_SetTarget(Player);
	}
	

	MonsterStat->IncreaseHP(-FinalDamage);

	

	return FinalDamage;
}


void ANPC_Master::Die()
{

	if (HasAuthority() == true)//¼­¹ö¿¡¼­¸¸ ½ÇÇà
	{
		//Á×ÀÎ Player exp ÁÜ
		auto MyPlayer = Cast<ACharacter_Master>(AttackedPlayer);
		if (MyPlayer != nullptr)
		{

			auto MyPlayerState = Cast<AMyPlayerState>(MyPlayer->GetPlayerState());
			MyPlayerState->CharacterStat->Server_SetExp(MyPlayerState->CharacterStat->CurrentExp + MonsterStat->DropExp);
		}
	}

}

void ANPC_Master::DestroyMonster()
{
	Destroy();
}