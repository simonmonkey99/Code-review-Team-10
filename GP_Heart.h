// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartActor.h"
#include "GP_Heart.generated.h"

/**
 * 
 */
UCLASS()
class GP3_UPROJECT_API AGP_Heart : public ASmartActor
{
	GENERATED_BODY()

	
public:
	AGP_Heart();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class AGameModePrototype* GameMode;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* EnemyDetection;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
	int OverlappingEnemies = 0;

	UPROPERTY(EditAnywhere)
	float DetectionRange = 1500;

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsUnderAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsSafe();

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void HeartIsNotTarget();

	UFUNCTION(BlueprintImplementableEvent)
	void HeartTakeDamage();

protected:

	class UBillboardComponent* BillboardComp;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void PostInitializeComponents();
};
