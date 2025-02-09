// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PuzzlePlatforms.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

		UPROPERTY()
		FVector Location;

	UPROPERTY()
		FRotator Rotation;

	UPROPERTY()
		FVector BoxExtent;

};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;
	UPROPERTY()
		ACharacter_Master* Character;
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()


	UPROPERTY()
	bool bHitConfirmed;

	bool bHeadShot;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEPLATFORMS_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULagCompensationComponent();
	friend class ACharacter_Master;
	void ShowFramePackage(const FFramePackage& Package, const FColor & Color);

	//HitScan
	FServerSideRewindResult ServerSideRewind(class ACharacter_Master* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);
	
	//Projectile
	FServerSideRewindResult ProjectileServerSideRewind(class ACharacter_Master* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);
	FFramePackage GetFrameToCheck(ACharacter_Master* HitCharacter, float HitTime);
	//HitScan
	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(
	ACharacter_Master* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime,
		class AWeapon_Master* DamageCauser);
	//Projectile
	UFUNCTION(Server, Reliable)
		void ProjectileServerScoreRequest(
			ACharacter_Master* HitCharacter,
			const FVector_NetQuantize& TraceStart,
			const FVector_NetQuantize100& InitialVelocity,
			float HitTime);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SaveFramePackage(FFramePackage& Package);
	//Scanline
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);
	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ACharacter_Master* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation);
	//Projectile
	FServerSideRewindResult ProjectileConfirmHit(const FFramePackage& Package, ACharacter_Master* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);
	void CacheBoxPositions(ACharacter_Master* HitCharacter, FFramePackage& OutFramePackage);
	void MoveBoxes(ACharacter_Master* HitCharacter, const FFramePackage& Package);
	void ResetHitBoxes(ACharacter_Master* HitCharacter, const FFramePackage& Package);
	void EnabledCharacterMeshCollision(ACharacter_Master* HitCharacter, ECollisionEnabled::Type CollisionEnabled);
	void SaveFramePackage();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ACharacter_Master* Character;
		
	UPROPERTY()
	class AMyPlayerController* Controller;
	
	TDoubleLinkedList<FFramePackage> FrameHistory;

	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;
};
