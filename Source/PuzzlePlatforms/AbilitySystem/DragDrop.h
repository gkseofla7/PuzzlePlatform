// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDrop.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void CustomInitialize(TSubclassOf<class AAbility> NewAbilityClass);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AAbility> AbilityClass;
	
};
