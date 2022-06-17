// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GP_EnemyPrototype.generated.h"


UCLASS()
class GP3_UPROJECT_API AGP_EnemyPrototype : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGP_EnemyPrototype();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:	

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float Damage(float DamageAmount);

	virtual void GoToHeart(AActor* Heart);

	virtual void GoToHeartAfterXSec();

	virtual void GoToPlayer();

	//virtual void GoToLastKnownLocation();

	//virtual void GoToGroundPos();

	virtual void AttackPlayer();

	virtual void AttackHeart();

	void StartStuckTimer();

	void StopStuckTimer();

	void DestroyAfterXSec();

	void TurnOffInvincibility();

	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
	class APlayerController* PlayerController;

	UPROPERTY()
	class AGPPlayerController* GPPlayerController;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AttackSphere;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* DetectSphere;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensor;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Hitbox;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class ASmartActor* SmartActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class AActor* Heart;

	UPROPERTY()
	class UGP_HealthComponent* HealthComp;

	UPROPERTY(EditAnywhere)
	class AActor* HealthActor;

	UPROPERTY(VisibleAnywhere)
	class AGameModePrototype* GameModeClass;

	class AGPWaveManager* Wavemanager;

	UPROPERTY(VisibleAnywhere)
	class UGP_PlayerGroundPosition* PlayerGroundPosition;

	UPROPERTY()
	class UCharacterMovementComponent* CharMoveComponent;


	UPROPERTY(VisibleAnywhere)
	float CurHealth = 0;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
	float AttackSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
	float FirstAttackDelay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
	float AttackRange = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
	float HeartAttackDamage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
	float PlayerAttackDamage = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DetectionRange = 500;

	UPROPERTY(EditAnywhere)
	float GoToHeartSpeed = 300;

	UPROPERTY(EditAnywhere)
	float ChasePlayerSpeed = 1200;

	UPROPERTY(EditAnywhere)
	bool StopToAttack = false;

	bool PossiblyStuck = false;

	bool Attacking = false;

	bool InHeartBox = false;

	UPROPERTY(EditAnywhere)
	float GoToHeartDelay = 3.0;

	UPROPERTY(EditAnywhere)
	float DestroyTimer = 5.0;

	UPROPERTY(VisibleAnywhere)
	bool Invincible = true;

	UPROPERTY(EditAnywhere)
	float InvincibilityTimer = 3.0;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> AllActors;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> HeartActors;

	UPROPERTY(VisibleAnywhere)
	FVector LastKnownLocation;

	FVector LastKnownVelocity;

	class UGPGameInstance* GameInstanceRef;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BloodSplatter;

	UPROPERTY(EditAnywhere)
	FVector BloodScale;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FTimerHandle TimerHandle;
	
	FTimerHandle StuckTimer;

	FTimerHandle InvincibilityTimerHandle;


	//////////////////////////////////////////////////////////////////////////
	// UPawnSensingComponent Delegates

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	UFUNCTION(BlueprintImplementableEvent)
	void Idle();

	UFUNCTION(BlueprintImplementableEvent)
	void Attack();

	UFUNCTION(BlueprintImplementableEvent)
	void Move();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyKilled();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemySpawn();

	UFUNCTION(BlueprintImplementableEvent)
	void DamagePlayer();

	virtual void PostInitializeComponents();


};