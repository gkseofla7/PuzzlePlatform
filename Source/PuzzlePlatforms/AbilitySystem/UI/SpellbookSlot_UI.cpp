// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellbookSlot_UI.h"
#include "../Ability.h"
#include "../../Character_Master.h"
#include "../../MyPlayerState.h"
#include "Spellbook_UI.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void USpellbookSlot_UI::CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass)
{
	AbilityClass = NewAbilityClass;

	auto ability = AbilityClass.GetDefaultObject();
	SpellImage->SetBrushFromTexture(ability->AbilityDetails.Icon);
}




bool USpellbookSlot_UI::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(UpgradeButton != nullptr)) return false;
	UpgradeButton->OnClicked.AddDynamic(this, &USpellbookSlot_UI::UpgradeSkill);
	auto PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr)//�ڱ��ڽ� ������
		return false;
	auto MyPlayerState = Cast<AMyPlayerState>(PlayerRef->GetPlayerState());
	if (MyPlayerState == nullptr) return false;

	MyPlayerState->OnSkillPointChangedDelegate.AddUObject(this, &USpellbookSlot_UI::SetSkillPoint);
	return true;

}

void USpellbookSlot_UI::UpgradeSkill()
{
	
	auto PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr)//�ڱ��ڽ� ������
		return;
	auto MyPlayerState = Cast<AMyPlayerState>(PlayerRef->GetPlayerState());
	if (MyPlayerState->SkillPoints <= 0)//SkillPoint�� 1�̻��϶���
		return;
	auto ability = AbilityClass.GetDefaultObject();
	if (MyPlayerState->SpellsUpgrade[SlotNum] == ability->AbilityMaxLevel)//�ش� ��ų�� �ִ� ���� �����ε� �̰� �ٲ���ߵ�
		return;

	MyPlayerState->Server_SpellsUpgrade(SlotNum);//�� �ѹ�¦ ��������..����
	//MyPlayerState->Server_SetSkillPoints(MyPlayerState->SkillPoints - 1);
	DisableBar->SetPercent(0.);
	

}

void USpellbookSlot_UI::SetSkillPoint()//��� UI�� �����
{
	auto PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr)//�ڱ��ڽ� ������
		return;
	auto MyPlayerState = Cast<AMyPlayerState>(PlayerRef->GetPlayerState());
	if (MyPlayerState == nullptr) return;

	FString FS_Level = FString::Printf(TEXT("%d"), MyPlayerState->SpellsUpgrade[SlotNum]);

	auto ability = AbilityClass.GetDefaultObject();
	if (ability->AbilityMaxLevel == MyPlayerState->SpellsUpgrade[SlotNum])
	{
		FS_Level = FString::Printf(TEXT("Max"));
	}
	T_UpgradeNum->SetText(FText::FromString(FS_Level));

	SpellbookRef->SetSkillPoints(MyPlayerState->SkillPoints);

}