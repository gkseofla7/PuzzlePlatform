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
	//void AbilitySpawn(class ACharacter_Master* NewPlayer);
	UActionBarSlotWidget(const FObjectInitializer& ObjectInitializer);
	//virtual void PreConstruct(bool IsDesignTime) override;
	virtual void NativePreConstruct();
	void NativeTick(const FGeometry& MyGeometry,	float InDeltaTime	) override;
	void IconInitialize();
	void StartCooldown();
	void EndCooldown();
	void UpdateAppearance();
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
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
	class ACharacter_Master* Owner;
	FTimerHandle CooldownTimerHandler;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UActionBarWidget* ParentsWidget;
	
};
