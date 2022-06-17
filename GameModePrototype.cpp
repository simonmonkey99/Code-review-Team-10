// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModePrototype.h"
#include "GPWaveManager.h"
#include "GP_Heart.h"
#include "GP_EnemyPrototype.h"



AGameModePrototype::AGameModePrototype()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGameModePrototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckEnemyStuck();
}

void AGameModePrototype::CheckEnemyStuck()
{
	if (EnemyArray.Num() - 1 >= EnemyIndex)
	{
		AGP_EnemyPrototype* Enemy = Cast<AGP_EnemyPrototype>(EnemyArray[EnemyIndex]);
		FVector VelocityDiff = EnemyArray[EnemyIndex]->GetVelocity() - Enemy->LastKnownVelocity;
		Enemy->LastKnownVelocity = EnemyArray[EnemyIndex]->GetVelocity();

		if (EnemyArray[EnemyIndex]->GetVelocity().Size() <= .1f && VelocityDiff.Size() <= .1f
			&& !Enemy->PossiblyStuck && !Enemy->Attacking && !Enemy->InHeartBox)
		{
			Enemy->StartStuckTimer();
		}

		else if (EnemyArray[EnemyIndex]->GetVelocity().Size() >= .1f && Enemy->PossiblyStuck || Enemy->Attacking || Enemy->InHeartBox) 
		{
			Enemy->StopStuckTimer();
		}

		EnemyIndex++;
	}

	else 
	{
		//This should prob not happen instantly, maybe start a timer? Idk
		EnemyIndex = 0;
	}
}

void AGameModePrototype::AddHeart(AActor* HeartActor)
{
	HeartArray.Add(HeartActor);
}

void AGameModePrototype::SelectedHearts()
{
	for (size_t i = 0; i < HeartArray.Num(); i++)
	{
		if (Wavemanager->spawnLocs[i].ActiveState == true && !SelectedHeartsArray.Contains(HeartArray[i]))
		{
			SelectedHeartsArray.Add(HeartArray[i]);
			Cast<AGP_Heart>(HeartArray[i])->HeartIsTarget();
		}

		if (Wavemanager->spawnLocs[i].ActiveState == false && SelectedHeartsArray.Contains(HeartArray[i]))
		{
			SelectedHeartsArray.Remove(HeartArray[i]);
			Cast<AGP_Heart>(HeartArray[i])->HeartIsNotTarget();
		}
	}
}

void AGameModePrototype::ClearHearts()
{
	for (size_t i = 0; i < SelectedHeartsArray.Num(); i++)
	{
		SelectedHeartsArray.Remove(HeartArray[i]);
		Cast<AGP_Heart>(HeartArray[i])->HeartIsNotTarget();
	}
}

