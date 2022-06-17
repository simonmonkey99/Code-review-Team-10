// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_EnemyPrototype.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "SmartActor.h"
#include "Kismet/GameplayStatics.h"
#include "GP_HealthComponent.h"
#include "GPPlayerController.h"
#include "GameModePrototype.h"
#include "GPWaveManager.h"
#include "GP_PlayerGroundPosition.h"
#include "GPGameInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP_Heart.h"
#include "Components/BoxComponent.h"


// Sets default values
AGP_EnemyPrototype::AGP_EnemyPrototype()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurHealth = MaxHealth;

	//Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	Capsule = FindComponentByClass<UCapsuleComponent>();
	if (Capsule) {
		RootComponent = Capsule;
	}

	//else
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("No Maidens ")));

	Hitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(RootComponent);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetSphereRadius(AttackRange); 	//Why doesn't this work here????

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DetectSphere->SetSphereRadius(DetectionRange);

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor Epic"));
}

// Called when the game starts or when spawned
void AGP_EnemyPrototype::BeginPlay()
{
	Super::BeginPlay();

	EnemySpawn();

	Idle();

	GameModeClass = (AGameModePrototype*)GetWorld()->GetAuthGameMode();

	Wavemanager = GameModeClass->Wavemanager;

	GameModeClass->EnemyArray.Add(this);

	GameInstanceRef = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	HeartActors = GameModeClass->HeartArray;

	if (CurHealth != MaxHealth)
	{
		CurHealth = MaxHealth;
	}

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGP_EnemyPrototype::GoToHeartAfterXSec, 1.f, false, GoToHeartDelay);

	Invincible = true;

	GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, this, &AGP_EnemyPrototype::TurnOffInvincibility, 1.f, false, InvincibilityTimer);

	//GoToHeart(Heart);

	//I DON't GET HOW DELEGATES WORK :(((
	//AIController->ReceiveMoveCompleted;

	// Only works here :(
	AttackSphere->SetSphereRadius(AttackRange);
	DetectSphere->SetSphereRadius(DetectionRange);

	PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController->GetPawn())
	{
		PlayerGroundPosition = PlayerController->GetPawn()->FindComponentByClass<UGP_PlayerGroundPosition>();
	}

	AIController = Cast<AAIController>(GetController());

	CharMoveComponent = FindComponentByClass<UCharacterMovementComponent>();

	//Probably don't need to get ALL actors of a scene here
	
	/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	
	for (size_t i = 0; i < AllActors.Num(); i++)
	{
		if (AllActors[i]->GetName() == "HealthActorBP_2")
		{
			HealthActor = AllActors[i];
		}
	}*/

	HealthActor = GameModeClass->HealthComponent;
	
	//This should get the nearest Heart I think
	if (!Heart)
	{
		float distance = 0;
		AActor* tempHeart = nullptr;

		for (size_t i = 0; i < GameModeClass->HeartArray.Num(); i++)
		{
			FVector a = GameModeClass->HeartArray[i]->GetActorLocation();
			FVector b = GetActorLocation();

			float c = (a - b).Size();

			if (c < distance || distance == 0)
			{
				distance = c;
				tempHeart = GameModeClass->HeartArray[i];
			}
		}

		Heart = tempHeart;
	}

	//List with GP_Heart* array in Gamemode, hearts register in begin play, maybe weak pointer 

	if (HealthActor)
		HealthComp = HealthActor->FindComponentByClass<UGP_HealthComponent>();

}

// Called every frame
void AGP_EnemyPrototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


float AGP_EnemyPrototype::Damage(float DamageAmount)
{
	if (!Invincible)
	{
		CurHealth -= DamageAmount;

		if (CurHealth <= 0)
		{
			if (GameInstanceRef)
				GameInstanceRef->AddHighScore(10);

			else
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Game Instance Reference")));

			if (BloodSplatter)
				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSplatter, GetActorLocation(), GetActorRotation());
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatter, GetActorLocation(), GetActorRotation(), BloodScale);

			//
			if (Wavemanager)
			{
				Wavemanager->EnemyKilled();
			}

			if (GameModeClass->EnemyArray.Contains(this))
			{
				GameModeClass->EnemyArray.Remove(this);
			}

			EnemyKilled();

			Destroy();
		}

		return DamageAmount;
	}

	else 
	{
		return 0;
	}
}

//NOOO need for the Target remove when have time
void AGP_EnemyPrototype::GoToHeart(AActor* Target)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Going To Heart")));
	if (!AIController)
	{
		AIController = Cast<AAIController>(GetController());
	}

	if (AIController)
	{

		if (AIController->MoveToActor(Target))
		{
			Move();
			CharMoveComponent->MaxWalkSpeed = GoToHeartSpeed;
			AIController->SetFocus(Heart);
		}

		else {
			Idle();
		}
		
	}

	else {
		Idle();
	}

}

void AGP_EnemyPrototype::GoToHeartAfterXSec()
{
	if (Heart)
	{
		if (AIController->MoveToActor(Heart))
		{
			Move();

			AIController->SetFocus(Heart);
		}

		else {
			Idle();
		}
	}

	//GetWorldTimerManager().ClearTimer(TimerHandle);
}



void AGP_EnemyPrototype::GoToPlayer()
{
	if (AIController)
	{

		if (AIController->MoveToActor(PlayerController))
		{
			CharMoveComponent->MaxWalkSpeed = ChasePlayerSpeed;
			
			Move();

			AIController->SetFocus(PlayerController);
			//

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s"), AIController->MoveToActor(PlayerController)));

			//Sometimes causes crash? I don't get when tho 
			if (PlayerController->GetPawn() && PlayerGroundPosition)
			{
				//LastKnownLocation = PlayerController->GetPawn()->GetActorLocation();
				
				LastKnownLocation = PlayerGroundPosition->GetFloor();
			}

			else 
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Can't get Player Owner")));
		}

		else {
			//This doesn't happen :)))) Scratch that it SOMETIMES happens i dunno i dont get it 
			GoToHeart(Heart);
		}
	}
}

//This is not at all working as well as I want it to. Maybe I should just make them go to the playerlocation and get the ground position  of where they are somehow
//Right now just using this as a backup incase they can't get to the ground pos either

/*void AGP_EnemyPrototype::GoToLastKnownLocation()
{
	if (AIController)
	{
		if (AIController->MoveToLocation(LastKnownLocation))
		{
			Move();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Going to last known location of player")));
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Can't get to last known location")));
			GoToHeart(Heart);
		}
	}
}*/



/*void AGP_EnemyPrototype::GoToGroundPos()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("FREEZE!2")));

	if (AIController)
	{
		if (PlayerGroundPosition)
		{
			if (AIController->MoveToLocation(PlayerGroundPosition->GetFloor()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Going to ground pos")));
				Move();
				LastKnownLocation = PlayerGroundPosition->GetFloor();
			}

			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Can't get to ground pos")));
				GoToLastKnownLocation();
			}
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No PlayerGroundPosition")));
		}
	}
}*/

void AGP_EnemyPrototype::AttackPlayer()
{
	if (HealthComp)
		HealthComp->TakeDamage(PlayerAttackDamage);

	AIController->SetFocus(PlayerController);

	Attacking = true;

	DamagePlayer();

	//They should stop here,
}

void AGP_EnemyPrototype::AttackHeart()
{
	Attacking = true;

	if (HealthComp) {
		HealthComp->TakeDamage(HeartAttackDamage);
	}

	Cast<AGP_Heart>(Heart)->HeartTakeDamage();
}

void AGP_EnemyPrototype::StartStuckTimer()
{
	GetWorldTimerManager().SetTimer(StuckTimer, this, &AGP_EnemyPrototype::DestroyAfterXSec, 1.0, false, DestroyTimer);
	PossiblyStuck = true;
}

void AGP_EnemyPrototype::StopStuckTimer()
{
	GetWorldTimerManager().ClearTimer(StuckTimer);
	PossiblyStuck = false;
}

void AGP_EnemyPrototype::DestroyAfterXSec()
{
	if (BloodSplatter)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatter, GetActorLocation(), GetActorRotation(), BloodScale);

	if (Wavemanager)
	{
		Wavemanager->EnemyKilled();
	}

	if (GameModeClass->EnemyArray.Contains(this))
	{
		GameModeClass->EnemyArray.Remove(this);
	}

	EnemyKilled();

	Destroy();
}

void AGP_EnemyPrototype::TurnOffInvincibility()
{
	Invincible = false;
}


void AGP_EnemyPrototype::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp == AttackSphere)
	{
		if (Cast<AGPPlayerController>(OtherActor)) {
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AGP_EnemyPrototype::AttackPlayer, AttackSpeed, true, FirstAttackDelay);
			Attack();
			if (StopToAttack)
				AIController->StopMovement();
		}

		if (OtherActor == Heart && OtherComp != Cast<AGP_Heart>(Heart)->EnemyDetection && OtherComp != Cast<AGP_Heart>(Heart)->BoxCollider)
		{
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AGP_EnemyPrototype::AttackHeart, AttackSpeed, true, FirstAttackDelay);
			Attack();
			AIController->StopMovement();
			AIController->SetFocus(Heart);
		}

		if (OtherActor == Heart && OtherComp == Cast<AGP_Heart>(Heart)->BoxCollider)
		{
			InHeartBox = true;
		}
	}


	if (OverlappedComp == DetectSphere)
	{
		if (Cast<AGPPlayerController>(OtherActor)) {
			GoToPlayer();
		}
	}
	
}

void AGP_EnemyPrototype::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OverlappedComp == AttackSphere)
	{
		//Same here
	//Also you can bug out the attack by quickly entering and leaving it's range, getting it stuck on the heart without attacking
		if (Cast<AGPPlayerController>(OtherActor)) {
			GetWorldTimerManager().ClearTimer(TimerHandle);
			if (StopToAttack) 
				GoToPlayer();

			
		}

		if (OtherActor == Heart && OtherComp != Cast<AGP_Heart>(Heart)->EnemyDetection) {
			Attacking = false;
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}

		if (OtherActor == Heart && OtherComp == Cast<AGP_Heart>(Heart)->BoxCollider)
		{
			InHeartBox = false;
		}
			
		//Check if enemy is looking at/attacking player first, so they don't awkwardly turn around everytime you run away from them
	}
	
	if (OverlappedComp == DetectSphere)
	{
		if (Cast<AGPPlayerController>(OtherActor)) {
			GoToHeart(Heart);
			Attacking = false;
			//GoToLastKnownLocation();
			//GoToGroundPos();
		}
	}
}



void AGP_EnemyPrototype::OnSeePawn(APawn* OtherPawn)
{
	//This looks kind of spaghetti but it works
	/*if (GoToPlayer())
	{
		
	}

	else {
		GoToHeart(Heart);	
	}*/
}

//Not sure if we need this function at all.
void AGP_EnemyPrototype::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HEARD PAWN")));
}


void AGP_EnemyPrototype::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	PawnSensor->OnSeePawn.AddDynamic(this, &AGP_EnemyPrototype::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AGP_EnemyPrototype::OnHearNoise);
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AGP_EnemyPrototype::OnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AGP_EnemyPrototype::OnOverlapEnd);
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AGP_EnemyPrototype::OnOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AGP_EnemyPrototype::OnOverlapEnd);
}

