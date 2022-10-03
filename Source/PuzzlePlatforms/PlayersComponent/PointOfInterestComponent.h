// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointOfInterestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable)
class PUZZLEPLATFORMS_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPointOfInterestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetIcon();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void AddPOI();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconImage;
		
};
