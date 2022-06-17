#pragma once
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"

#include "GPWaveManager.generated.h"

class AGPEnemySpawnLocation;

struct SpawnLoc
{
	SpawnLoc(FString name, bool activeState)
	{
		Name = name;
		ActiveState = activeState;
	}
	
	FString Name;
	bool ActiveState;
};

UCLASS()
class GP3_UPROJECT_API AGPWaveManager : public AActor
{
	GENERATED_BODY()
public:
	FTimerHandle TimerHandle;

	AGPWaveManager();

	int GetRandomSpawnPoint(TArray<AGPEnemySpawnLocation*> Heart);
	int GetRandomSpawnLocation();
	int RandomSpawnPoint(TArray<AGPEnemySpawnLocation*> Heart);

	UPROPERTY()
	FString AttackStateText;

	UFUNCTION(BlueprintCallable)
	FString GetAttackText();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrintAttackText();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetActiveSpawnLocations();

	UFUNCTION(BlueprintCallable)
	FString ChooseMixDialogueAndCombine();

	UFUNCTION(BlueprintCallable)
	FString ChooseSingleDialogue();
	
	
int RandomSpawnOrders[6][3]{
{1,2,0},
{1,0,2},
{2,1,0},
{2,0,1},
{0,1,2},
 {0,2,1} };
	int SpawnOrder[3];
	
	int spawnOrder;
	UPROPERTY(VisibleAnywhere, Category = "DEBUG")
	int LocationAmount;

	int LocationRest;
	int SelectedSpawnPoint;
	TArray<TArray<AGPEnemySpawnLocation*>>SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* AttackedHeartsSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* BeforeFirstwaveSound;

	UPROPERTY(EditAnywhere, Category="HeartSpawnPoints")
		TArray<AGPEnemySpawnLocation*> Heart1;

	UPROPERTY(EditAnywhere, Category = "HeartSpawnPoints")
		TArray<AGPEnemySpawnLocation*> Heart2;

	UPROPERTY(EditAnywhere, Category = "HeartSpawnPoints")
		TArray<AGPEnemySpawnLocation*> Heart3;
	void SpawnEnemies();


	
	
	UPROPERTY(EditDefaultsOnly)
		TArray<int> ActivatedSpawnPoint={};


	float timeBetweenSpawns=10;
	UPROPERTY(VisibleAnywhere)
	float TimeBetween;

	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
		float TimeBetweenWaves = 15;

	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
		float SecondsBetweenSpawns = 1;
	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
		float StartOfGameDelay = 7.0f;
	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
		float FirstWaveDelay = 15.0f;


	int Remaining;

	
	void StartOfGame();

	UFUNCTION(BlueprintImplementableEvent)
		void startOfGameEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void EndOfWave();

	//Needs to be connected to enemy death
	UPROPERTY(VisibleAnywhere)
		int AliveEnemies;

	UPROPERTY(EditAnywhere)
		int WaveSize = 10;

	UPROPERTY(EditAnywhere, Category = "Waves")
	TArray<int> IncreaseWaveBy;

	UPROPERTY(EditAnywhere, Category = "Waves")
	TArray<int>ActiveSpawnLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
		int WaveIndex = 1;
	

	
	void BeginPlay() override;
	
	void Pending();
	

	int TimesToSpawn;
	int Counter;
	void EnemyKilled();
	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
	int ActiveSpawnPoints1=2;
	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
	int ActiveSpawnPoints2=2;
	UPROPERTY(EditAnywhere, Category = "DESIGNERS")
	int ActiveSpawnPoints3=2;

	
	bool bRandomActiveSpawnPoints = false;

	UPROPERTY(VisibleAnywhere, Category = "DEBUG")
	int rest;

	UPROPERTY(VisibleAnywhere, Category = "DEBUG")
		int DoneSpawnLocations;

	//UPROPERTY(VisibleAnywhere, Category = "DEBUG")
	//int ActiveSpawnLocations = 1;

	UPROPERTY(VisibleAnywhere, Category = "DEBUG")
	int SpawnPointAmount;

	//bool bSpawnLocations[3];

	TArray<SpawnLoc> spawnLocs;

	
	
	TArray<AActor*> SpawnPoints;
	UPROPERTY(VisibleAnywhere)
	TArray<AGPEnemySpawnLocation*> someSpawnPoints;
	UPROPERTY(VisibleAnywhere)
	AGPEnemySpawnLocation* tmp;
	FString SpawnPoint;
	

	int counter1=0;
	int counter2=0;
	int counter3=0;

	


		int LocationWaveSize;


		
			TArray<AGPEnemySpawnLocation*> SpawnPointCopy1;
	
		int SpawnPointIndex1;

	
			TArray<AGPEnemySpawnLocation*> SpawnPointCopy2;
	
		int SpawnPointIndex2;

	
			TArray<AGPEnemySpawnLocation*> SpawnPointCopy3;
		
		int SpawnPointIndex3;
	
		int DoneSpawnPoints1;
		int DoneSpawnPoints2;
		int DoneSpawnPoints3;

		bool active1=true;
		bool active2 = true;
		bool active3 = true;
		bool nextSpawnLocation;
			void setwave();
			int randomSpawnLocation;
			bool newLocation;
		


			bool bRestSpawned;
	
	
	
			int LocationIndex=0;
			int SpawnPointIndex;
};
