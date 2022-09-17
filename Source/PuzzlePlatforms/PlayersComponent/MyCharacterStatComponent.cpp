// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "../DataTable/MyPlayerData.h"
#include "../PuzzlePlatformsCharacter.h"
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
	//auto PawnRef = Cast<APawn>(GetOwner());
	//SetNewLevel(Level);
	// 
	//if (PawnRef->HasAuthority()&&PawnRef->IsLocallyControlled() == true)
	//	SetNewLevel(Level);
	//else if (PawnRef->HasAuthority() == false && PawnRef->IsLocallyControlled() == true)
	//{
	//	//만들고 유포
	//}
	//else if(PawnRef->HasAuthority()  && PawnRef->IsLocallyControlled() == true)
	//	Server_BringServerState();

	// ...
	
}

void UMyCharacterStatComponent::InitializeComponent() //Post Initialize 전에 일어남 이새끼 내생각엔 서버에서 한번 실행할때 일언
{
	Super::InitializeComponent();



 }

void UMyCharacterStatComponent::SetNewLevel(int32 NewLevel)
{

	auto PawnRef = Cast<APawn>(GetOwner());


	auto MyGameInstance = Cast<UPuzzlePlatformsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//if (MyGameInstance == nullptr)
	//{
	//	return;
	//}
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);
	if (MyGameInstance != nullptr)
	{
		

		//Server_SetStatData(CurrentStatData);
	}

	if (nullptr != CurrentStatData)
	{

		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;//처음에만 해주기위해..ㅋㅋ
		OnHPChanged.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("SetNewLevel : %f"), CurrentStatData->MaxHP);
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
	//ABCHECK(nullptr != CurrentStatData, 0.5f);
	if (CurrentStatData == nullptr)
		return .5f;

	return CurrentHP / CurrentStatData->MaxHP;
	//return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
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
	CurrentHP = NewHp;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		Cast<APuzzlePlatformsCharacter>(GetOwner())->Die();
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