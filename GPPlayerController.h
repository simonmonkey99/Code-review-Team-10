// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
//#include "GameFramework/Character.h"
#include "GPPlayerController.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class USceneComponent;
class UFloatingPawnMovement;
class UGunInput;
class UGPAbility;

UCLASS()
class GP3_UPROJECT_API AGPPlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "values")
	FVector2D PitchMinMax = FVector2D(-50,50);
	
	//movement
	UPROPERTY(EditAnywhere, Category = "values")
	float RunningSpeed = 1000;

	UPROPERTY(EditAnywhere, Category = "values")
	float DashForce = 500;

	UPROPERTY(EditAnywhere, Category = "values")
	float dashCoolDownTime = 1;

	UPROPERTY(EditAnywhere, Category = "values")
	float AngleToApplyVerticalDash = 40;
	
	UPROPERTY(EditAnywhere, Category = "values")
	float VerticalDashForce = 500;

	UPROPERTY(EditAnywhere, Category = "values")
	float FloatingGravityScale = 0.1f;

	UPROPERTY(EditAnywhere, Category = "values")
	float FallGravityScale = 1.5f;
	
	UPROPERTY(BlueprintReadOnly)
	bool CanDash;
	
	//movement options bool
	UPROPERTY(EditAnywhere, Category = "values")
	bool CanRun = true;

	UPROPERTY(EditAnywhere, Category = "values")
	bool CanFloat = true;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	//GunInput
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;



	UPROPERTY()
	UGPAbility* Ability;

	
private:
	UPROPERTY()
	float defaultWalkSpeed;

	UPROPERTY()
	float defaultGravityScale;

	UPROPERTY()
	bool isFloating;

	UPROPERTY()
	bool dashPressed;

	UPROPERTY()
	FTimerHandle dashCooldownTimer;
	
protected:
	void MoveForward(float moveValue);
	void MoveRight(float moveValue);
	void PitchCamera(float val);

	void OnRunStart();
	void OnRunStop();

	UFUNCTION()
	void OnStartJump();
	
	UFUNCTION()
	void OnStopJump();
	
	void DashPressed();
	
	void SetCanDashTrue();
	void SetGroundFrictionDefault();

	void HandleActivateAbility();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	AGPPlayerController();

	UFUNCTION(BlueprintCallable)
	void OnDash();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDashEnabledEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFloatingEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFallingEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLerpFloatingVelToZero();
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
