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
#include "BehaviorTree/BlackboardComponent.h"

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
	if (Cast< ANPC_Master>(DamageCauser) != nullptr)
		return 0.f;//같은팀이면 패스~
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
	Cast<ANPCAIController>(GetController())->SetIsHitKeyTrue();//주변애들도 이거 셋팅해줌~
	GetHelpedFromOthers(Player);
	auto Archer = Cast<ANPC_Archer>(this);
	if (Archer != nullptr)
	{
		Archer->NetMulticast_SetTarget(Player);
	}
	


	if (DamageCauser != nullptr)
	{
		FVector OposDir = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
		LaunchCharacter(OposDir * 1000, false, false);
	}


	NetMulticast_DamageImpact(FinalDamage);


	return FinalDamage;
}

void ANPC_Master::GetHelpedFromOthers(ACharacter_Master * Target)
{

	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();//내위치
	float DetectRadius = 2000.f;//내주변 범위..! 도와줘, 테스트 차원에서 크게 잡음

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = World->OverlapMultiByChannel(OverlapResults,
		Center, FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);//channel objecttype profile

	if (bResult)//범위 내에 몬스터 없으면~ 초기화! 근데 범위 멀면 어찌하냐,,
	{

		for (auto const& OverlapResult : OverlapResults)//이미 존재하면 걔로 계속감
		{
			ANPCAIController* MyController = Cast<ANPCAIController>(GetController());
			auto Current = MyController->GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
			//bool same = false;
			ANPC_Master* Monster = Cast< ANPC_Master>(OverlapResult.GetActor());

			if (Monster != nullptr)
			{
				ANPCAIController* MonsterController = Cast<ANPCAIController>(Monster->GetController());
				auto MonsterTarget = MonsterController->GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
				if (MonsterTarget != nullptr)
					continue;//이미 때리는애 있으면 패스~


				auto Archer = Cast<ANPC_Archer>(Monster);
				if (Archer != nullptr)
				{
					Archer->NetMulticast_SetTarget(Target);
				}
				MonsterController->GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, Target);
				MonsterController->SetIsHitKeyTrue();
				

				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);


			}
		}
	}
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

