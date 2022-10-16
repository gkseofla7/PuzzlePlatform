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
#include "../Etc/CharDamageText.h"
#include "../Etc/SoulItem.h"

#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "BrainComponent.h"

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
	static ConstructorHelpers::FClassFinder<ACharDamageText> CharDamageTextBPClass(TEXT("/Game/Blueprint/Etc/BP_CharDamageText"));
	if (CharDamageTextBPClass.Succeeded())
	{
		CharDamageTextClass = CharDamageTextBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ASoulItem> SoulItemtBPClass(TEXT("/Game/Blueprint/Etc/BP_SoulItem"));
	if (SoulItemtBPClass.Succeeded())
	{
		SoulItemtClass = SoulItemtBPClass.Class;
	}

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Powerup"));
	if (ParticleAsset.Succeeded())
	{;
		ParticleTemplate = ParticleAsset.Object;
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
	UE_LOG(LogTemp, Warning, TEXT("Damaged holyground"));
	if (bDead == true)
		return 0;
	if (!HasAuthority())
		return 0;

	//ABCHECK(MotionReplicator != nullptr)

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AttackedPlayer = DamageCauser;
	auto Player = Cast< ACharacter_Master>(DamageCauser);
	MonsterStat->IncreaseHP(-FinalDamage);
	Cast<ANPCAIController>(GetController())->SetTargetKey(Player);
	Cast<ANPCAIController>(GetController())->SetIsHitKey(true);
	auto Archer = Cast<ANPC_Archer>(this);
	if (Archer != nullptr)
	{
		Archer->NetMulticast_SetTarget(Player);
	}
	



	FVector OposDir = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
	LaunchCharacter(OposDir * 1000, false, false);

	NetMulticast_DamageImpact(FinalDamage);


	return FinalDamage;
}

void ANPC_Master::ChangeDamageColor()
{

}
void ANPC_Master::DamageImpact(float Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Impact"));
	FTransform PlayerTransform = GetActorTransform();
	FActorSpawnParameters Params;
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner = GetOwner();
	SpawnInfo.Instigator = Cast<APawn>(GetOwner());
	auto DamageText = GetWorld()->SpawnActor<ACharDamageText>(CharDamageTextClass, PlayerTransform, SpawnInfo);
	DamageText->SetDamageText(Damage);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), FRotator(0, 0, 0));
	UGameplayStatics::SpawnEmitterAttached(ParticleTemplate, GetCapsuleComponent());
}

void ANPC_Master::NetMulticast_DamageImpact_Implementation(float Damage)
{

	DamageImpact(Damage);
	ChangeDamageColor();//이건 각각 정의해 줘야됨
	if (HasAuthority())
	{
		auto AIController = Cast< ANPCAIController>(GetController());

		//Montage 실행 및 비헤이비어 트리 정지
		if (bDead == true)
			return;
		AIController->PauseLogic();

	}
	if(bDead == false)
		PlayImpactMontage();//이것도 각자
}

bool ANPC_Master::NetMulticast_DamageImpact_Validate(float Damage)
{
	return true;
}


void ANPC_Master::PlayImpactMontage()
{
	
}


void ANPC_Master::Die()
{
	if (HasAuthority() == true)
	{
		Cast<ANPCAIController>(GetController())->BrainComponent->StopLogic("Die");
		FTimerHandle TimerHandler;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ANPC_Master::DestroyMonster, 4, false);
	}
	//if (HasAuthority() == true)//서버에서만 실행

}

void ANPC_Master::Destroyed()
{
	if (HasAuthority())
	{
		auto MyPlayer = Cast<ACharacter_Master>(AttackedPlayer);
		if (MyPlayer != nullptr)
		{
			int quantity = UKismetMathLibrary::RandomIntegerInRange(1, 3);
			SpawnLoot(quantity);
		}

	}

}

void ANPC_Master::SpawnLoot(int Quantity)
{
	for (int i = 0; i < Quantity; i++)
	{
		auto MyLoc = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(40, 40, 40));
		FTransform MyTransform;
		MyTransform.SetLocation(MyLoc);
		FActorSpawnParameters Params;
		FActorSpawnParameters SpawnInfo;

		SpawnInfo.Owner = AttackedPlayer;
		UE_LOG(LogTemp, Warning, TEXT("Spawn Soul Item"));
		auto SoulItem = GetWorld()->SpawnActor<ASoulItem>(SoulItemtClass, MyTransform, SpawnInfo);
		SoulItem->CustomInitialize(MonsterStat->DropExp);
	}
}


void ANPC_Master::DestroyMonster()
{
	Destroy();
	HPBarWidget->DestroyComponent();
}

