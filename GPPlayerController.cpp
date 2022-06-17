// Fill out your copyright notice in the Description page of Project Settings.

#include "GPPlayerController.h"

#include "GunInput.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GPAbility.h"
#include "GunInput.h"
#include "Kismet/KismetMathLibrary.h"

AGPPlayerController::AGPPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	//playerCameraComponent->bUsePawnControlRotation = true;
	
	

	defaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	defaultGravityScale = GetCharacterMovement()->GravityScale;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	//GunInputs
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	//dashPressed = false;
	CanDash = true;
}

void AGPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		true), TEXT("GripPoint"));
}

void AGPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1, 0, FColor::Yellow,
			FString::SanitizeFloat(GetCharacterMovement()->GravityScale));	

	//MOVEMENT
	//Set gravity scale to default when the player is on the Ground
	if(GetCharacterMovement()->IsMovingOnGround())
	{
		GetCharacterMovement()->GravityScale = defaultGravityScale;
	}

	//if floating set gravity to FLOATING
	if(isFloating && GetCharacterMovement()->Velocity.Z < 0)
	{
		GetCharacterMovement()->GravityScale = FloatingGravityScale;
		OnFloatingEvent();
	}

	//If no longer floating set gravity to FALLING.
	if(!isFloating && !GetCharacterMovement()->IsMovingOnGround())
	{
		GetCharacterMovement()->GravityScale = FallGravityScale;
		OnFallingEvent();
	}
	
	//ABILITY
	if(Ability != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(
			INDEX_NONE,
			0.f,
			FColor::Emerald,
			Ability->GetClass()->GetName(),
			true,
			FVector2D(4.f));
	}
}

void AGPPlayerController::MoveForward(float moveValue)
{
	const FVector direction = GetActorForwardVector();
	AddMovementInput(direction, moveValue, true);
}

void AGPPlayerController::MoveRight(float moveValue)
{
	const FVector direction = GetActorRightVector();
	AddMovementInput(direction, moveValue, true);
}

void AGPPlayerController::PitchCamera(float val)
{
	float currentRot = CameraComponent->GetComponentRotation().Pitch;
	
	const FRotator rot = FRotator(val, 0 ,0);
	if(currentRot + rot.Pitch < PitchMinMax.X || currentRot + rot.Pitch > PitchMinMax.Y)
		return;
	
	CameraComponent->AddLocalRotation(rot);
}

void AGPPlayerController::DashPressed()
{
	//dashPressed = (dashPressed == true) ? false : true;
}

void AGPPlayerController::OnDash()
{
	if(!CanDash)
		return;

	FVector direction;
	float dashForceTemp = DashForce;

	if(GetCharacterMovement()->IsMovingOnGround() && CameraComponent->GetComponentRotation().Pitch < 0)
	{
		direction = GetActorForwardVector();
	}
	else if(CameraComponent->GetComponentRotation().Pitch > AngleToApplyVerticalDash)
	{
		direction = GetActorUpVector();
		DashForce = VerticalDashForce;
	}
	else
	{
		direction = CameraComponent->GetForwardVector();
	}
	
	//GetCharacterMovement()->GroundFriction = 0;
	LaunchCharacter(direction * DashForce * 10, false, false);

	CanDash = false;
	DashForce = dashForceTemp;
	
	GetWorldTimerManager().SetTimer(dashCooldownTimer, this, &AGPPlayerController::SetCanDashTrue, dashCoolDownTime, false);
}

void AGPPlayerController::SetCanDashTrue()
{
	OnDashEnabledEvent();
	CanDash = true;
}

void AGPPlayerController::SetGroundFrictionDefault()
{
	GetCharacterMovement()->GroundFriction = 8;
}

void AGPPlayerController::OnRunStart()
{
	if(!CanRun)
		return;
	
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AGPPlayerController::OnRunStop()
{
	if(!CanRun)
		return;
	
	GetCharacterMovement()->MaxWalkSpeed = defaultWalkSpeed;
}

void AGPPlayerController::OnStartJump()
{
	isFloating = true;
	OnLerpFloatingVelToZero();
	//GetCharacterMovement()->Velocity.Z = 0;
	bPressedJump = true;
}

void AGPPlayerController::OnStopJump()
{
	isFloating = false;
	bPressedJump = false;
}

void AGPPlayerController::HandleActivateAbility()
{
	if(Ability)
		Ability->OnAbilityActivated();
}

void AGPPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	//movements
	PlayerInputComponent->BindAxis("GPMoveForward", this, &AGPPlayerController::MoveForward);
	PlayerInputComponent->BindAxis("GPMoveRight", this, &AGPPlayerController::MoveRight);
	
	//Running
	PlayerInputComponent->BindAction("GPRun", IE_Pressed, this, &AGPPlayerController::OnRunStart);
	PlayerInputComponent->BindAction("GPRun", IE_Released, this, &AGPPlayerController::OnRunStop);

	//Dashing
	//PlayerInputComponent->BindAction("GPDash", IE_Pressed, this, &AGPPlayerController::DashPressed);
	//PlayerInputComponent->BindAction("GPDash", IE_Released, this, &AGPPlayerController::DashPressed);
	
	//Rotation (Camera for pitch)
	PlayerInputComponent->BindAxis("GPLookUp", this, &AGPPlayerController::PitchCamera);
	PlayerInputComponent->BindAxis("GPTurnRight", this, &AGPPlayerController::AddControllerYawInput);

	//Jumping
	PlayerInputComponent->BindAction("GPJump", IE_Pressed, this, &AGPPlayerController::OnStartJump);
	PlayerInputComponent->BindAction("GPJump", IE_Released, this, &AGPPlayerController::OnStopJump);

	//fire moved to playercontroller bp
	//PlayerInputComponent->BindAction("GPFire", IE_Pressed, this, &AGPPlayerController::StartGunFire);
	//PlayerInputComponent->BindAction("GPFire", IE_Released, this, &AGPPlayerController::StopGunFire);

	//Ability
	PlayerInputComponent->BindAction(
		"GPUseAbility", IE_Pressed, this, &AGPPlayerController::HandleActivateAbility);

	
}

