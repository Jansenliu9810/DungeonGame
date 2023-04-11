// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerActor.h"

// Sets default values
AEnemySpawnerActor::AEnemySpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawnerActor::SpawnEnemy()
{

	// spawn new actor
	if (ClassToSpawn)	// ClassToSpawn not nullptr
	{
		// evaluate the enemy spawn number is over the limit or not
		if (EnemyCounter + EnemyDeathNum < LifeTimeNumber)
		{
			if (EnemyCounter < SpawnLimit)
			{
				// create a stack variable Params of type FActorSpawnParameters
				FActorSpawnParameters Params;

				// use UWorld object to evaluate the currently level is not nullptr
				if (UWorld* World = GetWorld())
				{
					// spawn enemy, use Cast to transit the class from AActor to AEnemyActor
					// If the transition failed, then return nullptr by default
					AEnemyCharacter* PC = Cast<AEnemyCharacter>(World->SpawnActor<AActor>(
						ClassToSpawn,
						GetActorLocation(),
						GetActorRotation(),
						Params
						));

					if (PC != nullptr)
					{
						// if PC pointer not equal to nullptr, that bind the function CountEnemyEliminated to OnEnemyEliminated
						PC->OnEnemyEliminated.BindUObject(this, &AEnemySpawnerActor::CountEnemyEliminated);
					}
				}
				EnemyCounter++;
			}
		}
		else if(EnemyDeathNum == LifeTimeNumber)
		{

			FActorSpawnParameters Params;

			// use UWorld object to evaluate the currently level is not nullptr
			if (UWorld* World = GetWorld())
			{
				// spawn enemy, use Cast to transit the class from AActor to AEnemyActor
				// If the transition failed, then return nullptr by default
				ASpecialItem* itemActor = Cast<ASpecialItem>(World->SpawnActor<AActor>(
					ItemToSpawn,
					GetActorLocation(),
					GetActorRotation(),
					Params
					));
			}
			EnemyDeathNum++;
		}

	}

}

// Called when the game starts or when spawned
void AEnemySpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
	// make a stack variable is TimerDelegate of type FTimerDelegate
	FTimerDelegate TimerDelegate;
	// Bind the object this
	TimerDelegate.BindUObject(this, &AEnemySpawnerActor::SpawnEnemy);

	// Timer looping control
	FTimerManager& TimerManager = GetWorldTimerManager();

	TimerManager.SetTimer(
		SpawnTimerHandle,
		TimerDelegate,	// Second param is TimerDelegate of the type FTimerDelegate
		6.f,
		true,
		5
	);


}

// Called every frame
void AEnemySpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemySpawnerActor::CountEnemyEliminated()
{
	EnemyCounter--;
	EnemyDeathNum++;
}