// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"

#include "PlayerCharacter.generated.h"

// declare a delegate to print itemlist
DECLARE_DELEGATE(FPrintItemList)

UCLASS()
class ASSIGNMENT2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void StopAttacking();

	UFUNCTION(BlueprintImplementableEvent)
	bool IsAttacking();

	// Create a function to bind about Player attack the capsule of Enemy
	UFUNCTION()
	void AttackingCapsule(
		UPrimitiveComponent* HitComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& Hit
	);


	// Create a function to bind about Player attach the Special Item
	UFUNCTION()
		void AttachItem(
			UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& Hit
		);

	int damageAmount;

	void PrintSpecialItem();

	// Exit Game Function
	void ExitGame();

	// create a delegate about FPrintItemList
	FPrintItemList printItemList;

private:

	// Listen to this sword's overlap events to deal damage to enemies.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SwordMesh;

	// create a FScriptDelegate variable
	FScriptDelegate SwordDelegate;

	// Listen to this special item events
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SpecialItemMesh;

	FScriptDelegate bindItemDelegate;

	int countItemPickUp = 0;

};
