// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.h"

#include "EnemyCharacter.generated.h"

// Declare a delegate type as FOnEnemyElimintated
DECLARE_DELEGATE(FOnEnemyEliminated)

UCLASS()
class ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Bind to the Overlap and Endoverlap delegates of this component to run logic to go to the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerDetectionSphere;

	// Bind to the Overlap and Endoverlap delegate of this component to run logic to attack the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttackRadiusSphere;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRadius = 500.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// A Damage Done function
	UFUNCTION()
	void DamageDone();

	void DoDamage(int howMuch);

	
	// create a variable of FOnEnemyElimintated
	FOnEnemyEliminated OnEnemyEliminated;


	// Enemy Movement Part, functions information
	UFUNCTION()
	void StartChaseTarget(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& Hit
	);

	UFUNCTION()
	void StopChaseTarget(
			UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& Hit
		);

	// Aim to make the enemy Look at the player location on time
	FRotator GetFocusRotationYaw(FVector target);
	FRotator lookAtYaw;

private:

	int health = 100;
	bool bIsAlive = true;
	int reduceHealth;

	// Enemy Movement members
	FScriptDelegate bindEnemyStartChase;
	FScriptDelegate bindEnemyStopChase;

	FVector distanceToPlayer;
	FVector enemyPosition;
	FVector playerPosition;

	bool inRange = false;

	UPROPERTY(EditAnywhere, Category = "Chasing Speed")
	float chasingSpeed = 150.f;

};

