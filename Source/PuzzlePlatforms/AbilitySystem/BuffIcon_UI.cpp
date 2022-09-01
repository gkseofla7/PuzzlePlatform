// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffIcon_UI.h"

#include "Ability_Buff_Master.h"
#include "BuffPanel_UI.h"

#include "Components/WrapBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"



void UBuffIcon_UI::CustomInitialize( AAbility_Buff_Master* NewBuffAbility)
{
	BuffAbility = NewBuffAbility;
	BuffImage->SetBrushFromTexture(BuffAbility->AbilityDetails.Icon);
	BuffLifeSpan = BuffAbility->BuffLifeSpan;
	BuffAbility->OnEndBuffDelegate.AddUObject(this, &UBuffIcon_UI::DeleteFromParent);


	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &UBuffIcon_UI::UpdateProgressBar, .5, true);

	
}

void UBuffIcon_UI::UpdateProgressBar()
{
	if (BuffAbility != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Percent : %f/%f"), BuffAbility->TimeSpend, BuffLifeSpan)
		float percent = UKismetMathLibrary::NormalizeToRange(BuffAbility->TimeSpend, 0, BuffLifeSpan);
		//percent = 1 - percent;
		BuffTimer->SetPercent(percent);
		if (BuffAbility->TimeSpend- BuffLifeSpan<0.1)
		{

			UE_LOG(LogTemp, Warning, TEXT("Clear"));
			GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
			
		}

	}


}

void UBuffIcon_UI::DeleteFromParent()
{
	//Cast< UBuffPanel_UI>(GetParent())->BuffPanel
	RemoveFromParent();
	Destruct();
}