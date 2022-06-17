// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GP_PlayerGroundPosition.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_UPROJECT_API UGP_PlayerGroundPosition : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGP_PlayerGroundPosition();

protected:
	virtual void BeginPlay() override;

public:	
	
	class UCharacterMovementComponent* CharMovementComp;

	class AGPPlayerController* PlayerController;

	class APlayerController* PlayerController2;

	virtual FVector GetFloor();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* GroundSphere;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SphereMesh;

	
};
