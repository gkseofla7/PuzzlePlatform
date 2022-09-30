// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include"../Character_Master.h"
#include "../UI/HPBarWidget.h"
#include "TurretStatComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../MenuSystem/QuitMenu.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	StaticMeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBase"));
	StaticMeshTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshTop"));
	StaticMeshBarrelL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBarrelL"));
	StaticMeshBarrelR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBarrelR"));
	RootComponent = StaticMeshBase;
	StaticMeshTop->SetupAttachment(StaticMeshBase,"TopSocket");
	StaticMeshBarrelL->SetupAttachment(StaticMeshTop, "LeftBarrelSocket");
	StaticMeshBarrelR->SetupAttachment(StaticMeshTop, "RightBarrelSocket");

	TurretStat = CreateDefaultSubobject<UTurretStatComponent>(TEXT("TurretStat"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(StaticMeshTop);

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
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	auto TurretWidget = Cast< UHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != TurretWidget)
	{
		TurretWidget->BindTurretStat(TurretStat);
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
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

	FindBestTarget();
	if (Target != nullptr)
	{
		UpdateTurretRotation(DeltaTime);
		if (HasAuthority())
		{
			UpdateFire();
		}
	}
}

void ATurret::FindBestTarget()
{
	float BestDistance = SensingRange + 1;
	AActor* BestTarget = nullptr;
	AActor* CurrentTarget = nullptr;
	float CurrentDistance = 0;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	IgnoreActors.Add(this);
	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SensingRange, ObjectTypes, nullptr, IgnoreActors, OutActors);

	if (isOverlapped)
	{

		for (int i = 0; i < OutActors.Num(); i++)
		{
			
				
			CurrentTarget = OutActors[i];
			auto PuzzleCharacter = Cast<ACharacter_Master>(CurrentTarget);
			if (PuzzleCharacter != nullptr)
			{
				if (PuzzleCharacter->TeamNum == TeamNum)
					continue;//넘김
			}
			CurrentDistance = CurrentTarget->GetDistanceTo(this);
			if (CurrentDistance < BestDistance || BestTarget==nullptr)
			{
				
				TArray<AActor*> IgnoreActorsLinetrace;
				IgnoreActorsLinetrace.Add(this);
				IgnoreActorsLinetrace.Add(CurrentTarget);

				bool tmp = HasLineOfSight(StaticMeshTop->GetComponentLocation(), CurrentTarget->GetActorLocation(), IgnoreActorsLinetrace);

				if (tmp == true)
				{
					
					BestTarget = CurrentTarget;
					BestDistance = CurrentDistance;
				}
			}
		}
	}
	Target = BestTarget;
}

bool ATurret::HasLineOfSight(FVector From, FVector To, TArray<AActor*> ActorsToIgnore)
{
//	UKismetSystemLibrary::LineTraceSingle	= > 내부적으로는 	UWorld::LineTraceSingleByChannel
	//FVector Target = AimObejctFPP->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(TurretTrace), false);
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	return !(GetWorld()->LineTraceSingleByChannel(Hit, From, To, ECC_Visibility, QueryParams));
}

void ATurret::UpdateTurretRotation(float DeltaSeconds)
{
	FRotator L_Current = StaticMeshTop->GetComponentRotation();
	FRotator L_Target = UKismetMathLibrary::FindLookAtRotation(StaticMeshTop->GetComponentLocation(), Target->GetActorLocation());

	FRotator NewRotation = FMath::RInterpConstantTo(L_Current, L_Target, DeltaSeconds, RotateSpeed);
	float Y = FMath::ClampAngle(NewRotation.Pitch, -45, 75);
	//StaticMeshTop->SetWorldRotation(NewRotation.Roll, Y, NewRotation.Yaw);
	NewRotation.Pitch = Y;
	StaticMeshTop->SetWorldRotation(NewRotation);

}
void ATurret::UpdateFire()
{
	float tmp = UGameplayStatics::GetTimeSeconds(GetWorld())- TimeOfLastFire;
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	if (tmp > RateOfFire )
	{

		FVector L_From = StaticMeshTop->GetComponentLocation();
		FVector L_Target = Target->GetActorLocation();
		FVector TurretDirection = UKismetMathLibrary::GetDirectionUnitVector(L_From, L_Target);
		if (FVector::DotProduct(TurretDirection,StaticMeshTop->GetForwardVector()) > .99)
		{
			NetMulticast_Fire();
		}
	}
}

void ATurret::NetMulticast_Fire_Implementation()
{
	Fire();
}

bool ATurret::NetMulticast_Fire_Validate()
{
	return true;
}

float ATurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{


	if (!HasAuthority())
		return 0;

	//ABCHECK(MotionReplicator != nullptr)

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	float HP = TurretStat->CurrentHp;

	TurretStat->Server_SetHp(HP - FinalDamage);

	return FinalDamage;

}


void ATurret::NetMulticast_DestroyEffect_Implementation()
{
	if (DestroyImpactParticles != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyImpactParticles, GetTransform());
	}

	auto MyGameInstance = Cast< UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstance->LoadCloseMenu();
	MyGameInstance->GetQuitMenu()->EndGame(TeamNum);
}


bool ATurret::NetMulticast_DestroyEffect_Validate()
{
	return true;
}