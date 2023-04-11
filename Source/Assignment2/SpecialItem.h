// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialItem.generated.h"

// Given some item type by enumerate
UENUM(BlueprintType)
enum SpecialItemType
{
	weaponClass UMETA(DisplayName = "Weapon"),
	coinClass UMETA(DisplayName = "Money"),
	bottleClass UMETA(DisplayName = "Drug"),
	nullType UMETA(DisplayName = NULL)
};

// create a struct to store itemName and itemType
USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	FString itemName;
	FString itemType;
};

UCLASS(Blueprintable)
class ASSIGNMENT2_API ASpecialItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpecialItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Create a FString variable to store the name of special item
	UPROPERTY(EditAnywhere)
	FString specialItemName;

	// Create two function to return specialItemName and specialItemType for FString class
	FString ItemName();
	FString ItemType(const SpecialItemType value);

	// A Function to Destory Item after it picked up
	UFUNCTION()
	void DestoryItem();

	UFUNCTION()
	void ItemInfoStore(FString typeInfo);

	void PrintItemInfo();

private:
	//TArray<FItemInfo> itemList;

	TMap<FString, FString> itemList;


};
