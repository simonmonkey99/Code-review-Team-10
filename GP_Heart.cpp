// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_Heart.h"
#include "GameModePrototype.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "GP_EnemyPrototype.h"

AGP_Heart::AGP_Heart()
{
	BillboardComp = FindComponentByClass<UBillboardComponent>();
	if (BillboardComp) {
		RootComponent = BillboardComp;
	}

	EnemyDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection"));
	EnemyDetection->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);
}

void AGP_Heart::BeginPlay()
{
	Super::BeginPlay();

	EnemyDetection->SetSphereRadius(DetectionRange);

	GameMode = Cast<AGameModePrototype>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
		GameMode->AddHeart(this);

	else 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Gamemode found in Heart.cpp")));
}

void AGP_Heart::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp == EnemyDetection)
	{
		if (Cast<AGP_EnemyPrototype>(OtherActor))
		{
			if (OverlappingEnemies == 0)
			{
				HeartIsUnderAttack();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HeartIsUnderAttack()")));
			}

			OverlappingEnemies++;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(OverlappingEnemies));
		}
	}
}

void AGP_Heart::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp == EnemyDetection)
	{
		if (Cast<AGP_EnemyPrototype>(OtherActor))
		{
			if (OverlappingEnemies == 1)
			{
				HeartIsSafe();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HeartIsSafe()")));
			}

			OverlappingEnemies--;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(OverlappingEnemies));
		}
	}
}

void AGP_Heart::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyDetection->OnComponentBeginOverlap.AddDynamic(this, &AGP_Heart::OnOverlapBegin);
	EnemyDetection->OnComponentEndOverlap.AddDynamic(this, &AGP_Heart::OnOverlapEnd);
}
