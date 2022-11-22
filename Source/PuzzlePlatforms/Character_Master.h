// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include"PuzzlePlatforms.h"
#include "GameFramework/Character.h"

#include "PlayersComponent/SoldierMotionReplicator.h"
#include "PlayersComponent/CharacterMotionReplicator.h"
#include "AbilitySystem/UI/CharacterSpellbookInterface.h"


#include "Character_Master.generated.h"


enum ECharacterType
{
	Soldier,
	Warrior
}; 


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptCastingDeleagate);
DECLARE_MULTICAST_DELEGATE(FOnSkillReleased);

UCLASS(config=Game, Abstract, BlueprintType)
class ACharacter_Master : public ACharacter, public ICharacterSpellbookInterface
{
protected:
	GENERATED_BODY()

public:
	ACharacter_Master();
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MoveRight(float Value);
	virtual void AddControllerPitchInput(float Val);
	virtual void AddControllerYawInput(float Val);
	
private:
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Skill1Clicked();
	void Skill2Clicked();
	void Skill3Clicked();
	void Skill4Clicked();
	void Skill5Clicked();
	void SkillReleased();
	void OpenSkillTree();
	void OpenMap();
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetIcon();
	virtual TArray<TSubclassOf<class AAbility>> GetPlayerSpells() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	void UpdateStat();//이거 hp의 경우엔 서버에도 해줘야됨;
	FRotator GetMuzzleRotation();
	void BindCharacterStatToWidget();
	void Sprint();
	void UnSprint();
	void SetbIsAttacking(bool NewbIsAttacking);
	void SetbUsingSkill(bool NewbUsingSkill);
	void DestroyPlayer();
	void UnvisiblePlayer();
	virtual void Attack();
	virtual void Die();
	void SetPlayerStat();
	void 	OnRep_PlayerState() override;


	UFUNCTION(BlueprintImplementableEvent)
		void AddCrosshairWidget();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY()
		TMap<FName, class UBoxComponent*> HitCollisionBoxes;

public:
	virtual void MoveForward(float Value);
	UPROPERTY()
	TSubclassOf<class UCameraShakeBase>CameraShakeClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* NearObjectCollisionDetector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

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
	UPROPERTY()
	class UPlayerInfoWidget* PlayerInfoHUDWidget;
	UPROPERTY()
	ACharacter_Master* TargetPlayer;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInterruptCastingDeleagate InterruptCasting;
	FOnSkillReleased OnSkillReleased;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UFPSTargetWidget* CrosshairWidget;
	UPROPERTY()
		class UAnimInstance_Master* MyAnim;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;
	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;
	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;
	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;
	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;
	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;
	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;
	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;
	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;
	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;
	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l;
	UPROPERTY(VisibleAnywhere)
		class ULagCompensationComponent* LagCompensation;


	FTimerHandle StatResetHandle;

	UPROPERTY(Replicated)
	int Level;//입장과 동시에 Replicated 됨 단 서버쪽에서는 아니겠지..ㅋㅋ
	UPROPERTY(EditAnywhere)
	bool bPossessed = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TeamNum = 0;
	bool bDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CharacterNum = 0;
	float GeneralWalkSpeed = 400;
	float SteamPackWalkSpeed = 600	;
	bool ChangeIcon = false;

	bool bIsInRespawnSection = false;
	bool bSkillAvailable = true;
	bool bMouseCursorToggle = false;
	bool bIsDashing = false;
	bool bIsSprinting = false;
	bool bIsAttacking = false;
	bool bUsingSkill = false;
	ECharacterType CharacterType;
	float Pitch;
	float Yaw;
	TQueue<TTuple<FVector,float>> LocationHistory;

};

