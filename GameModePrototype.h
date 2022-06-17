// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModePrototype.generated.h"

/**
 * 
 */
UCLASS()
class GP3_UPROJECT_API AGameModePrototype : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AGameModePrototype();

	TArray<AActor*> HeartArray;

	TArray<AActor*> SelectedHeartsArray;

	TArray<AActor*> EnemyArray;

	TArray<FVector> EnemyVelocity;

	AActor* HealthComponent;


	class AGPWaveManager* Wavemanager;

	virtual void Tick(float DeltaTime) override;

	void CheckEnemyStuck();

	int EnemyIndex = 0;

	virtual void AddHeart(AActor* HeartActor);

	void SelectedHearts();

	void ClearHearts();

	FTimerHandle StuckTimer;
};
