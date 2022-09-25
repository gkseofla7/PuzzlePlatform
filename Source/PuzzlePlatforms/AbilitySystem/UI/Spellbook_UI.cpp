// Fill out your copyright notice in the Description page of Project Settings.


#include "Spellbook_UI.h"

#include "../../Character_Master.h"
#include "../ActorAbilities.h"
#include "SpellbookSlot_UI.h"

#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"


USpellbook_UI::USpellbook_UI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>	SpellbookSlotBPClass(TEXT("/Game/AbilitySystem/UI/Spellbook/BP_SpellbookSlot_UI"));
	if (!ensure(SpellbookSlotBPClass.Class != nullptr)) return;
	SpellbookSlotClass = SpellbookSlotBPClass.Class;
}

void 	USpellbook_UI::NativeConstruct()
{
	auto PlayerRef = Cast<ACharacter_Master>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr)
		return;
	auto SpellsRef = PlayerRef->ActorAbilitiesComponent->PlayerSpells;
	//UE_LOG(LogTemp, Warning, TEXT("Spellbook Num : %d "), SpellsRef.Num());
	//for (auto Spell : SpellsRef)
	//{
	//	USpellbookSlot_UI* Slot_UI = CreateWidget<USpellbookSlot_UI>(GetWorld(), SpellbookSlotClass);
	//	Slot_UI->CustomInitialize(Spell);
	//	Container->AddChildToWrapBox(Slot_UI);

	//}


	for (int i = 0; i < SpellsRef.Num(); i++)
	{

		USpellbookSlot_UI* Slot_UI = CreateWidget<USpellbookSlot_UI>(GetWorld(), SpellbookSlotClass);
		Slot_UI->CustomInitialize(SpellsRef[i]);
		Slot_UI->SlotNum = i;
		Container->AddChildToWrapBox(Slot_UI);

	}
}