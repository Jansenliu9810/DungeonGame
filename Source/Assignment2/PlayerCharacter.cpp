// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Assignment2GameMode.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "SpecialItem.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach a sword mesh to the character mesh
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
	SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// bind the Sword Component with AttackingCapsule function
	SwordDelegate.BindUFunction(this, FName("AttackingCapsule"));
	SwordMesh->OnComponentBeginOverlap.Add(SwordDelegate);

	// bind the SpecialItem Component with AttachItem function
	bindItemDelegate.BindUFunction(this, FName("AttachItem"));
	OnActorBeginOverlap.Add(bindItemDelegate);
	GetMesh()->OnComponentBeginOverlap.Add(bindItemDelegate);

	

}

// Called every frame 
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// check the number of items which is picked up
	PrintSpecialItem();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::AttackingCapsule(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComponent)
	{
		// Copy from Helpful Code Snippets
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
		{
			// Cast it to our specific game mode
			if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
			{
				// Determine if an attack is underway and if an EnemyCapsuleComponent has been hit
				if (Cast<UCapsuleComponent>(OtherComponent) != nullptr && IsAttacking())	
				{
					damageAmount = FMath::RandRange(10, 30);
					// Send a basic message
					FStringFormatNamedArguments formatAttackMessage;
					formatAttackMessage.Add(TEXT("AMOUNT"), FStringFormatArg(damageAmount));
					const FString NewMessage = FString::Format(TEXT("Yes I did {AMOUNT} damage"), formatAttackMessage);
					AssignmentGameMode->SendCharacterMessage(NewMessage);

					StopAttacking();

					// Hit the EnemyCharacter, and reduce it blood
					AEnemyCharacter* enemyActor = nullptr;
					if (Cast<AEnemyCharacter>(OtherActor) != nullptr)
					{
						enemyActor = Cast<AEnemyCharacter>(OtherActor);
						enemyActor->DoDamage(damageAmount);
					}
				}
			}
		}
	}
}


// Define a function about Player Attach Item to pick up
void APlayerCharacter::AttachItem(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ASpecialItem* pickUpItem = Cast<ASpecialItem>(OtherActor);
	if (pickUpItem)
	{
		// Get the game mode
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
		{
			// Cast it to our specific game mode
			if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
			{
				// Call the DestoryItem Function which defined in SpecialItem.cpp
				++countItemPickUp;
				pickUpItem->DestoryItem();


				// Send a basic message
				FStringFormatNamedArguments formatPickUpArrgument;
				// find the type of item
				if (pickUpItem->ItemName() == "Coin")
				{
					// ENUM value
					const SpecialItemType value = SpecialItemType::coinClass;
					pickUpItem->ItemInfoStore(pickUpItem->ItemType(value));

					// print out the congratulation info
					formatPickUpArrgument.Add(TEXT("TYPE"), pickUpItem->ItemType(value));
					formatPickUpArrgument.Add(TEXT("ITEMNAME"), pickUpItem->ItemName());
					FString pickUpMessage = FString::Format(TEXT("Yeah! I just picked up a {ITEMNAME}.\n It is {TYPE}!"), formatPickUpArrgument);
					AssignmentGameMode->SendCharacterMessage(pickUpMessage);
				}
				else if (pickUpItem->ItemName() == "King Shield")
				{
					// ENUM value
					const SpecialItemType value = SpecialItemType::weaponClass;
					pickUpItem->ItemInfoStore(pickUpItem->ItemType(value));
					
					formatPickUpArrgument.Add(TEXT("TYPE"), pickUpItem->ItemType(value));
					formatPickUpArrgument.Add(TEXT("ITEMNAME"), pickUpItem->ItemName());
					FString pickUpMessage = FString::Format(TEXT("Yeah! I just picked up a {ITEMNAME}.\n It is a {TYPE}!"), formatPickUpArrgument);
					AssignmentGameMode->SendCharacterMessage(pickUpMessage);
				}
				else if (pickUpItem->ItemName() == "Poison")
				{
					// ENUM value
					const SpecialItemType value = SpecialItemType::bottleClass;
					pickUpItem->ItemInfoStore(pickUpItem->ItemType(value));

					formatPickUpArrgument.Add(TEXT("TYPE"), pickUpItem->ItemType(value));
					formatPickUpArrgument.Add(TEXT("ITEMNAME"), pickUpItem->ItemName());
					FString pickUpMessage = FString::Format(TEXT("Yeah! I just picked up a {ITEMNAME}.\n It is a {TYPE}!"), formatPickUpArrgument);
					AssignmentGameMode->SendCharacterMessage(pickUpMessage);
				}
				else
				{
					const SpecialItemType value = SpecialItemType::nullType;
					pickUpItem->ItemInfoStore(pickUpItem->ItemType(value));

					formatPickUpArrgument.Add(TEXT("ITEMNAME"), pickUpItem->ItemName());
					FString pickUpMessage = FString::Format(TEXT("Yeah! I just picked up a {ITEMNAME}!\n So cooooool~"), formatPickUpArrgument);
					AssignmentGameMode->SendCharacterMessage(pickUpMessage);
				}


			}
		}
	}

}



void APlayerCharacter::PrintSpecialItem()
{
	
	// Set up the condition to exit the game
	if (countItemPickUp >= 4)
	{

		FString congraMess;		// congratulation message
		congraMess = TEXT("Congratulation! You already picked up all special items!");
		GEngine->AddOnScreenDebugMessage(0, 20.f, FColor::Green, congraMess, true);

		ASpecialItem* pointPrint = nullptr;
		pointPrint->PrintItemInfo();

		// print the items what we picked after 5s, then exit game after 10s
		FTimerHandle exitGameHandle;
		GetWorld()->GetTimerManager().SetTimer(exitGameHandle, this, &APlayerCharacter::ExitGame, 6, false);
	}
}

void APlayerCharacter::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("QUIT");
}