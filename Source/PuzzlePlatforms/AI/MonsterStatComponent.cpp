// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "NPC_Master.h"


// Sets default values for this component's properties
UMonsterStatComponent::UMonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true; //�̰� �ؾ� initializecomponent ����

	Level = 1;
}


// Called when the game starts
void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	SetNewLevel(Level);//beginplay�� ��� ���� ���涧��..
}


// Called every frame
//void UMonsterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}



void UMonsterStatComponent::InitializeComponent() //Post Initialize ���� �Ͼ
{
	Super::InitializeComponent();

}

void UMonsterStatComponent::CustomInitializeComponent(EMonsterEnum NewMonsterEnum)
{
	MonsterEnum = NewMonsterEnum;

}

void UMonsterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstance == nullptr)
		return;


	if (MyGameInstance != nullptr)
		CurrentStatData = MyGameInstance->GetMonsterData(NewLevel, MonsterEnum);

	if (nullptr != CurrentStatData)
	{
		CurrentHP = CurrentStatData->MaxHP;//ó������ ���ֱ�����..����
		OnHPChanged.Broadcast();
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		AttackDamage = CurrentStatData->Attack;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist"), NewLevel);
	}

}

void UMonsterStatComponent::SetHP(float NewHP)
{
	//������ �����ߵ�

	Server_SetHP(NewHP);
}



float UMonsterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}




void UMonsterStatComponent::Server_SetHP_Implementation(float NewHp)
{
	NetMulticast_SetHP(NewHp);
}

void UMonsterStatComponent::NetMulticast_SetHP_Implementation(float NewHp)
{

	CurrentHP = NewHp;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		Cast<ANPC_Master>(GetOwner())->Die();
		//OnHPIsZero.Broadcast();
	}
}

bool UMonsterStatComponent::Server_SetHP_Validate(float NewHp)
{
	return true;
}

bool UMonsterStatComponent::NetMulticast_SetHP_Validate(float NewHp)
{
	return true;
}