// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CastBarWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastSuccessfulDeleagate);
UCLASS()
class PUZZLEPLATFORMS_API UCastBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void StartCast();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CompletedCast();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void InterruptedCast();
	void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UProgressBar* CastBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAbility* Ability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACharacter_Master* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle CastTimer;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCastSuccessfulDeleagate CastSuccessful;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool WhileBuffering = false;
	
};
