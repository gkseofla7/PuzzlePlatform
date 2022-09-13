// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../DataTable/MyPlayerData.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true; //이걸 해야 initializecomponent 실행

	Level = 1;

	// ...
}



// Called when the game starts
void UMyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	// ...
	
}

void UMyCharacterStatComponent::InitializeComponent() //Post Initialize 전에 일어남
{
	Super::InitializeComponent();
	SetNewLevel(Level);
 }

void UMyCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (MyGameInstance == nullptr)
		return;

	if(MyGameInstance!=nullptr)
		CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		SetMP(CurrentStatData->MaxMP);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist"), NewLevel);
	}

}

void UMyCharacterStatComponent::SetHP(float NewHP)
{
	//서버로 보내야됨

	Server_SetHP(NewHP);
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


float UMyCharacterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

float UMyCharacterStatComponent::GetMPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxMP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentMP / CurrentStatData->MaxMP);
}


//void UMyCharacterStatComponent::OnRep_HP()
//{
//	OnHPChanged.Broadcast();
//}



void UMyCharacterStatComponent::Server_SetHP_Implementation(float NewHp)
{
	NetMulticast_SetHP(NewHp);
}

void UMyCharacterStatComponent::NetMulticast_SetHP_Implementation(float NewHp)
{

	CurrentHP = NewHp;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		//OnHPIsZero.Broadcast();
	}
}

bool UMyCharacterStatComponent::Server_SetHP_Validate(float NewHp)
{
	return true;
}

bool UMyCharacterStatComponent::NetMulticast_SetHP_Validate(float NewHp)
{
	return true;
}