// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionBarSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UActionBarSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void CastButtonClicked();
	void AbilitySpawn(class APuzzlePlatformsCharacter* NewPlayer);
	UActionBarSlotWidget(const FObjectInitializer& ObjectInitializer);
	//virtual void PreConstruct(bool IsDesignTime) override;
	virtual void NativePreConstruct();
	void NativeTick(const FGeometry& MyGeometry,	float InDeltaTime	) override;
	bool Initialize();
	void StartCooldown();
	void EndCooldown();
	void UpdateAppearance();
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* IconImage;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* CastButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UProgressBar* CooldownBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AAbility>AbilityClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAvailable = true;
	bool IsManaAvailable = true;
	class APuzzlePlatformsCharacter* Owner;
	FTimerHandle CooldownTimerHandler;
	
};
