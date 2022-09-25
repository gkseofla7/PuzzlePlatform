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
	return true;

}

void USpellbookSlot_UI::UpgradeSkill()
{
	
	auto PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr)//자기자신 가져옴
		return;
	auto MyPlayerState = Cast<AMyPlayerState>(PlayerRef->GetPlayerState());
	if (MyPlayerState->SkillPoints <= 0)//SkillPoint가 1이상일때만
		return;

	MyPlayerState->Server_SpellsUpgrade(SlotNum);
	FString FS_Level = FString::Printf(TEXT("%d"), MyPlayerState->SpellsUpgrade[SlotNum]);
	//MyPlayerState->SpellsUpgrade[SlotNum]++;
	T_UpgradeNum->SetText(FText::FromString(FS_Level));
	DisableBar->SetPercent(0.);
	MyPlayerState->Server_SetSkillPoints(MyPlayerState->SkillPoints - 1);
	SpellbookRef->SetSkillPoints(MyPlayerState->SkillPoints);
}