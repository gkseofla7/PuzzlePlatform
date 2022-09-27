// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../DataTable/MyPlayerData.h"
#include "../Character_Master.h"
#include "../MyPlayerState.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true; //이걸 해야 initializecomponent 실행

	Level = 1;

}



// Called when the game starts
void UMyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

void UMyCharacterStatComponent::InitializeComponent() //Post Initialize 전에 일어남 이새끼 내생각엔 서버에서 한번 실행할때 일언
{
	Super::InitializeComponent();
 }

void UMyCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		Cast<ACharacter_Master>(GetOwner())->bDead = true;
		Cast<ACharacter_Master>(GetOwner())->Die();
	}

}

void UMyCharacterStatComponent::SetMP(float NewMP)
{
	CurrentMP = NewMP;

	OnMPChanged.Broadcast();
	if (CurrentMP < KINDA_SMALL_NUMBER)
	{
		CurrentMP = 0.0f;
	}
}
void UMyCharacterStatComponent::SetExp(int NewExp)
{

	ABCHECK(CurrentStatData != nullptr);

	if (NewExp >= CurrentStatData->NextExp)
	{

		NewExp = NewExp - CurrentStatData->NextExp;
		Level++;
		SetLevel(Level);
	}
	UE_LOG(LogTemp, Warning, TEXT("SetEXP : %d"), NewExp);

	CurrentExp = NewExp;
	OnExpChanged.Broadcast();
}

void UMyCharacterStatComponent::SetLevel(float NewLevel)
{

	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ABCHECK(MyGameInstance != nullptr);
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//모든애들 가져옴, 꼭 이래야될까..?ㅋㅋ
	if (NewLevel == 1)
	{
		CurrentExp = 0;//0으로 초기화
	}
	if (nullptr != CurrentStatData)
	{
		
		Level = NewLevel;//BroadCast도 해줘야됨
		OnLevelChanged.Broadcast();
		SetHP(CurrentStatData->MaxHP);
		SetMP(CurrentStatData->MaxMP);
		AttackDamage = CurrentStatData->Attack;
		if (GetOwner()->HasAuthority())
		{
			auto MyPlayerState = Cast< AMyPlayerState>(GetOwner());
			MyPlayerState->Server_SetSkillPoints(MyPlayerState->SkillPoints + 1);
		}

		//Attack도 해줌
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist"), NewLevel);
	}
}

float UMyCharacterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.f);
	return CurrentHP / CurrentStatData->MaxHP;
}

float UMyCharacterStatComponent::GetMPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxMP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentMP / CurrentStatData->MaxMP);
}

float UMyCharacterStatComponent::GetExpRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return float(CurrentExp) / float(CurrentStatData->NextExp);
}


void UMyCharacterStatComponent::Server_SetHP_Implementation(float NewHp)
{
	NetMulticast_SetHP(NewHp);
}

void UMyCharacterStatComponent::NetMulticast_SetHP_Implementation(float NewHp)
{
	SetHP(NewHp);
}


void UMyCharacterStatComponent::Server_SetMP_Implementation(float NewMp)
{
	NetMulticast_SetMP(NewMp);
}



void UMyCharacterStatComponent::NetMulticast_SetMP_Implementation(float NewMp)
{
	SetMP(NewMp);
}



void UMyCharacterStatComponent::Server_SetLevel_Implementation(float NewLevel)
{
	NetMulticast_SetLevel(NewLevel);
}


void UMyCharacterStatComponent::NetMulticast_SetLevel_Implementation(float NewLevel)
{
	SetLevel(NewLevel);
}





void UMyCharacterStatComponent::Server_SetExp_Implementation(int NewExp)
{
	NetMulticast_SetExp(NewExp);
}

bool UMyCharacterStatComponent::Server_SetExp_Validate(int NewExp)
{
	return true;
}

void UMyCharacterStatComponent::NetMulticast_SetExp_Implementation(int NewExp)
{
	//음,, 더좋은 방법 없으려나;;
	SetExp(NewExp);
}



bool UMyCharacterStatComponent::NetMulticast_SetExp_Validate(int NewExp)
{
	return true;
}

bool UMyCharacterStatComponent::Server_SetHP_Validate(float NewHp)
{
	return true;
}

bool UMyCharacterStatComponent::NetMulticast_SetHP_Validate(float NewHp)
{
	return true;
}


bool UMyCharacterStatComponent::Server_SetMP_Validate(float NewHp)
{
	return true;
}
bool UMyCharacterStatComponent::NetMulticast_SetMP_Validate(float NewHp)
{
	return true;
}

bool UMyCharacterStatComponent::Server_SetLevel_Validate(float NewLevel)
{
	return true;
}

bool UMyCharacterStatComponent::NetMulticast_SetLevel_Validate(float NewLevel)
{
	return true;
}


