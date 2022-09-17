// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include"PuzzlePlatforms.h"
#include "GameFramework/Character.h"
//#include "PlayersComponent/MotionReplicatorInterface.h"
#include "PlayersComponent/SoldierMotionReplicator.h"
#include "PlayersComponent/MotionInterfaceComponent.h"
#include "PuzzlePlatformsCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptCastingDeleagate);
DECLARE_MULTICAST_DELEGATE(FOnSkillReleased);

UCLASS(config=Game, BlueprintType)
class APuzzlePlatformsCharacter : public ACharacter
{
protected:
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NearObjectCollisionDetector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	APuzzlePlatformsCharacter();
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	void UpdateStat();//이거 hp의 경우엔 서버에도 해줘야됨;
	FRotator GetMuzzleRotation();
	void Skill1Clicked();
	void Skill2Clicked();
	void Skill3Clicked();
	void Skill4Clicked();
	void Skill5Clicked();
	void SkillReleased();
	void SetIsAttacking(bool NewIsAttacking);
	void SetUsingSkill(bool NewUsingSkill);
	void DestroyPlayer();
	virtual void PossessedBy(AController* NewController) override;
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void GetInTheCar();
	virtual void Attack();
	virtual void AddControllerPitchInput(float Val);
	virtual void AddControllerYawInput(float Val);
	virtual void Die();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetLevel(int NewLevel);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SetLevel(int NewLevel);
	void SetStatComponentLevel();
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetServerLevel();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	void SetTargetPlayerWithLineTrace();




public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Stat)
		class UMyCharacterStatComponent* CharacterStat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		UMotionInterfaceComponent* DaerimMotionReplicator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UActorAbilities* ActorAbilitiesComponent;//받은 능력?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		class UDecalComponent* DecalComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	class UHudUpDisplayWidget* HeadsUpDisplayRef;
	APuzzlePlatformsCharacter* TargetPlayer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInterruptCastingDeleagate InterruptCasting;
	FOnSkillReleased OnSkillReleased;

	UPROPERTY()
		class UAnimInstance_Master* MyAnim;
	FTimerHandle StatResetHandle;
	bool SkillAvailable = true;
	bool MouseCursorToggle = false;
	bool IsDashing = false;
	bool IsAttacking = false;
	bool UsingSkill = false;
	UPROPERTY(Replicated)
	int Level;//입장과 동시에 Replicated 됨 단 서버쪽에서는 아니겠지..ㅋㅋ



};

