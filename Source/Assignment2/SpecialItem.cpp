// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialItem.h"
#include "Components/SphereComponent.h"

#include "PlayerCharacter.h"
#include "Assignment2GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpecialItem::ASpecialItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpecialItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpecialItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// return Item Name
FString ASpecialItem::ItemName()
{
	return FString(specialItemName);
}

FString ASpecialItem::ItemType(const SpecialItemType value)
{
	// Get ENUM Object
	const UEnum* EnumObject = FindObject<UEnum>(ANY_PACKAGE, TEXT("SpecialItemType"));
	// Get DisplayName
	FString DisplayInfo = EnumObject->GetMetaData(TEXT("DisplayName"), value);

	ItemInfoStore(DisplayInfo);

	return DisplayInfo;
}

// Destory Item
void ASpecialItem::DestoryItem()
{
	Destroy();
}

void ASpecialItem::ItemInfoStore(FString typeInfo)
{
	FItemInfo newItem;
	newItem.itemName = ItemName();
	newItem.itemType = typeInfo;
	itemList.Add(newItem.itemType, newItem.itemName);
}


// This is a function to print the list of special items before exiting the game. 
// Because UE5 compilation reported an error, I commented it out
void ASpecialItem::PrintItemInfo()
{
	//FString itemPrint;
	//for (auto item = itemList.CreateConstIterator(); item; ++item)
	//{
	//	itemPrint = FString("Item type: " + item.Key() + ", Item Name : " + item.Value());
	//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, itemPrint, true);
	//}
	
}







