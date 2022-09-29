// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include"PuzzlePlatforms.h"
#include "GameFramework/Character.h"

#include "PlayersComponent/SoldierMotionReplicator.h"
#include "PlayersComponent/CharacterMotionReplicator.h"


#include "Character_Master.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptCastingDeleagate);
DECLARE_MULTICAST_DELEGATE(FOnSkillReleased);

UCLASS(config=Game, BlueprintType)
class ACharacter_Master : public ACharacter
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
	ACharacter_Master();

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
	void OpenSkillTree();
	void OpenMap();
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
	//void Test();
	void SetPlayerStat();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_BindCharacterStatToWidget();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void  NetMulticast_BindCharacterStatToWidget();

	void BindCharacterStatToWidget();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	void SetTargetPlayerWithLineTrace();

	//UFUNCTION(BlueprintCallable)
	//	bool SaveRenderTarget(UTextureRenderTarget2D* renderTarget, FString path, FString fileName);




public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Stat)
		class UMyCharacterStatComponent* CharacterStatRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		UCharacterMotionReplicator* ReplicateComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UActorAbilities* ActorAbilitiesComponent;//받은 능력?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replicator)
		class UDecalComponent* DecalComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPointOfInterestComponent* PointOfInterestComponent;

	class UPlayerInfoWidget* PlayerInfoHUDWidget;
	ACharacter_Master* TargetPlayer;
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
	UPROPERTY(EditAnywhere)
	bool bPossessed = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TeamNum = 0;
	bool bDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CharacterNum = 0;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Server_SetPlayerStat();
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMulticast_SetPlayerStat();
};

