// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "AbilityDetailStruct.h"
#include "Ability.generated.h"



UCLASS()
class PUZZLEPLATFORMS_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void BeginCasting();
	UFUNCTION(BlueprintCallable)
		void CastAbility();
	virtual void CastAbility_Implementation() ;
	UFUNCTION(BlueprintCallable)
		void InterruptCast();
	UFUNCTION(BlueprintCallable)
		void ActivateEffect();
	virtual void ActivateEffect_Implementation();

	void DetachAbilityFromPlayer();
	UFUNCTION()
	void EndAnimation(UAnimMontage* Montage, bool bInterrupted);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityDetailsStruct AbilityDetails;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* AbilityRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APuzzlePlatformsCharacter* PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHudUpDisplayWidget* HudUI;

	class UAnimInstance_Master* AnimRef;

	class UCastBarWidget* CastBar;




};

