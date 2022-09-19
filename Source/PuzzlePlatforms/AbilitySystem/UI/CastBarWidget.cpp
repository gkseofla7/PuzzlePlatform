// Fill out your copyright notice in the Description page of Project Settings.


#include "CastBarWidget.h"
#include "../../PuzzlePlatformsCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ProgressBar.h"
void UCastBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<APuzzlePlatformsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(PlayerCharacter!= nullptr)
		PlayerCharacter->InterruptCasting.AddDynamic(this, &UCastBarWidget::InterruptedCast);
}
void UCastBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (IsVisible() == true)
	{
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CastTimer);
		float TotalTime = ElapsedTime + GetWorld()->GetTimerManager().GetTimerRemaining(CastTimer);
		float Percent = UKismetMathLibrary::NormalizeToRange(ElapsedTime, 0, TotalTime);
		CastBar->SetPercent(Percent);
	}
}