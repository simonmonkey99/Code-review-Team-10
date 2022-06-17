// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_PlayerGroundPosition.h"

#include "GPPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

UGP_PlayerGroundPosition::UGP_PlayerGroundPosition()
{
	PrimaryComponentTick.bCanEverTick = true;

	GroundSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GroundSphere"));
	GroundSphere->SetupAttachment(this);

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(this);
}


void UGP_PlayerGroundPosition::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AGPPlayerController>(GetOwner());

	PlayerController2 = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
}



FVector UGP_PlayerGroundPosition::GetFloor()
{
	FHitResult Hit;
	const float WeaponRange = 200000.f;
	const FVector StartTrace = GetOwner()->GetActorLocation();
	const FVector EndTrace = (GetOwner()->GetActorUpVector() * -1 * WeaponRange) + StartTrace;

	//FString HitString = (GetOwner()->GetFName()).ToString();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *HitString));

	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, GetOwner());

	//const FName TraceTag("MyTraceTag");

	//GetWorld()->DebugDrawTraceTag = TraceTag;

	//QueryParams.TraceTag = TraceTag;

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{

		return Hit.ImpactPoint;
	}

	else
		return StartTrace;
}

void UGP_PlayerGroundPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SphereMesh->SetWorldLocation(GetFloor(), false, 0, ETeleportType::None);
	
}

