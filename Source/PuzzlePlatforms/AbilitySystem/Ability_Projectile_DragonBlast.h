// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability_Projectile.h"
#include "Ability_Projectile_DragonBlast.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AAbility_Projectile_DragonBlast : public AAbility_Projectile
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
	virtual void CastAbility_Implementation() override;
	virtual void ActivateEffect_Implementation() override;
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UPlayerAnimInstance* AsPlayerAnimInstance;
	bool Toggle = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> SweepArray;
};
