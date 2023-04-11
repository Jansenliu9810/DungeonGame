// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"

#include "Assignment2GameMode.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectionSphere"));
	AttackRadiusSphere = CreateDefaultSubobject<USphereComponent>(FName("AttackRadiusSphere"));
	
	if (RootComponent)
	{
		if (AttackRadiusSphere)
		{
			AttackRadiusSphere->SetSphereRadius(AttackRadius);

			// Make sure it's attached to the root component
			AttackRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		
		if (PlayerDetectionSphere)
		{
			PlayerDetectionSphere->SetSphereRadius(PlayerDetectionRadius);

			// Make sure it's attached to the root component
			PlayerDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{

	Super::BeginPlay();

	// bind a Target of Interest to the PlayerDetectionSphere which included in CharacterMovementComponent.h file
	bindEnemyStartChase.BindUFunction(this, FName("StartChaseTarget"));
	PlayerDetectionSphere->OnComponentBeginOverlap.Add(bindEnemyStartChase);
	PlayerDetectionSphere->OnComponentEndOverlap.Add(bindEnemyStopChase);

	bindEnemyStopChase.BindUFunction(this, FName("StopChaseTarget"));
	AttackRadiusSphere->OnComponentBeginOverlap.Add(bindEnemyStopChase);
	AttackRadiusSphere->OnComponentEndOverlap.Add(bindEnemyStartChase);

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (inRange == true)
	{
		// Set Actor Rotation
		FRotator interpRotation = FMath::RInterpTo(this->GetActorRotation(), lookAtYaw, DeltaTime, 5.f);
		SetActorRotation(interpRotation);


		this->GetCharacterMovement()->MaxWalkSpeed = chasingSpeed;
		this->GetMovementComponent()->AddInputVector(distanceToPlayer);
		
	}

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AEnemyCharacter::DamageDone()
{

	// Copy from Helpful Code Snippets
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		// Cast it to our specific game mode
		if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
		{
			// Send a basic message
			const FString deathMessage = FString::Printf(TEXT("Ouch, it is dead for %d damage"), health);
			AssignmentGameMode->SendCharacterMessage(deathMessage);

		}
	}
	UE_LOG(LogTemp, Error, TEXT("Ouch, It took %d amount of damage!\n Now it is dead."), health);
}

void AEnemyCharacter::DoDamage(int howMuch)
{

	health -= howMuch;

	//OnActorHit.Broadcast(howMuch);
	if (bIsAlive && health <= 0)
	{
		// When the health less than equal to 0, that call the DamageDone function once
		bIsAlive = false;
		DamageDone();

		// active OnEnemyEliminated, tell the Spawner Enemy destroy
		OnEnemyEliminated.ExecuteIfBound();
		Destroy();

	}

}

// EnemyStartChasing function and EnemyStopChasing function definition

void AEnemyCharacter::StartChaseTarget(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
	if(playerActor)
	{
		if (Cast<UCapsuleComponent>(OtherComponent) != nullptr)
		{
			enemyPosition = this->GetActorLocation();
			playerPosition = playerActor->GetActorLocation();
			distanceToPlayer = playerActor->GetActorLocation() - this->GetActorLocation();

			lookAtYaw = GetFocusRotationYaw(playerActor->GetActorLocation());

			inRange = true;
		}
	}
}

void AEnemyCharacter::StopChaseTarget(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	APlayerCharacter* playerActor = Cast<APlayerCharacter>(OtherActor);
	if(playerActor)
	{
		if (Cast<UCapsuleComponent>(OtherComponent) != nullptr)
		{
			lookAtYaw = GetFocusRotationYaw(playerActor->GetActorLocation());

			distanceToPlayer = FVector(0);

			inRange = false;
		}
	}

}

// Set up the rotation of Enemy to Looking At target
FRotator AEnemyCharacter::GetFocusRotationYaw(FVector target)
{
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target);
	FRotator lookAtRotationYaw(0.f, lookAtRotation.Yaw, 0.f);
	return lookAtRotationYaw;
}
