// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../DataTable/MyPlayerData.h"
#include "../PuzzlePlatformsCharacter.h"
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

void UMyCharacterStatComponent::LevelUp(int32 NewLevel)//모든 애들이 다 이걸 실행함
{

	auto PawnRef = Cast<APawn>(GetOwner());
	ABCHECK(PawnRef != nullptr);
	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ABCHECK(MyGameInstance != nullptr);
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//모든애들 가져옴, 꼭 이래야될까..?ㅋㅋ

	if (nullptr != CurrentStatData)
	{

		Level = NewLevel;//모든애들 Level Set
		OnLevelChanged.Broadcast();
		//CurrentHP = CurrentStatData->MaxHP;//처음에만 해주기위해..ㅋㅋ
		//OnHPChanged.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("LevelUp : %f"), CurrentStatData->MaxHP);
		//전원 돌림..ㅋㅋ
		
		if (PawnRef->HasAuthority())
		{
			SetHP(CurrentStatData->MaxHP);
			SetMP(CurrentStatData->MaxMP);
		}
		if (PawnRef->IsLocallyControlled() && PawnRef->IsPlayerControlled())
		{
			auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GameInstance != nullptr)
			{
				Server_SetName(GameInstance->PlayerName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist"), NewLevel);
	}

}

void UMyCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		Cast<APuzzlePlatformsCharacter>(GetOwner())->Die();
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

void UMyCharacterStatComponent::SetLevel(float NewLevel)
{

	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ABCHECK(MyGameInstance != nullptr);
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//모든애들 가져옴, 꼭 이래야될까..?ㅋㅋ

	if (nullptr != CurrentStatData)
	{
		{
			Level = NewLevel;//BroadCast도 해줘야됨
		}
		OnLevelChanged.Broadcast();
		SetHP(CurrentStatData->MaxHP);
		SetMP(CurrentStatData->MaxMP);
		//Attack도 해줌

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist"), NewLevel);
	}
}

//void UMyCharacterStatComponent::SetName(FText NewName)
//{
//
//}


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

void UMyCharacterStatComponent::Server_SetName_Implementation(const FText& NewName)
{
	NetMulticast_SetName(NewName);
}

void UMyCharacterStatComponent::NetMulticast_SetName_Implementation(const FText& NewName)
{
	Name = NewName;
	OnNameChanged.Broadcast();
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


bool UMyCharacterStatComponent::Server_SetName_Validate(const FText& NewName)
{
	return true;
}

bool UMyCharacterStatComponent::NetMulticast_SetName_Validate(const FText& NewName)
{
	return true;
}


//void Server_SetStatData_Implementation(FMyCharacterrData* NewStatData)
//{
//	NetMulticast_SetStatData_Implementation(NewStatData);
//}
//bool Server_SetStatData_Validate(FMyCharacterrData* NewStatData)
//{
//	return true;
//}
//
//void NetMulticast_SetStatData_Implementation(FMyCharacterrData* NewStatData)
//{
//	CurrentStatData = NewStatData;
//}
//bool NetMulticast_SetStatData_Validate(FMyCharacterrData* NewStatData)
//{
//	return true;
//}