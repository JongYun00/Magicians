// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryActorComponent.h"

// Sets default values for this component's properties
UInventoryActorComponent::UInventoryActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FItemData UInventoryActorComponent::LoadItemFromDataTable(FName ItemRowName)
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable!"));
		return FItemData();	// 기본값 반환
	}
	
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("Item Lookup"));

	if (ItemData != nullptr)
	{
		return *ItemData;	// 유효한 데이터 반환
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("item with RowName %s not found in DataTable."), *ItemRowName.ToString());
		return FItemData();	// 기본값 반환
	}
	/*if (ItemDataTable)
	{
		TArray<FName> RowNames = ItemDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			
			if (ItemData != nullptr)
			{
				FInventoryItem newItem;
				newItem.ItemData = *ItemData;
				newItem.Quantity = 1;

				Inventory.Add(ItemData->ItemID, newItem);
			}
		}
	}*/
}

void UInventoryActorComponent::AddItemToInventory(FName ItemRowName, int32 Quantity)
{
	if (IsInventoryFull())
	{
		// 인벤토리가 가득 찼다면
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Full"));
		return;
	}

	// 이미 인벤토리에 아이템이 있다면
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ExistingItem = Inventory[ItemRowName];

		if (ExistingItem.ItemData.bIsStackable)
		{
			ExistingItem.Quantity += Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Increased quantity of item with RowName"), *ItemRowName.ToString(), ExistingItem.Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item with ID %s is not stackable!"), *ItemRowName.ToString());
		}
	}
	else
	{
		if (ItemDataTable)
		{
			FItemData ItemData = LoadItemFromDataTable(ItemRowName);
			FInventoryItem NewItem;
			NewItem.ItemData = ItemData;
			NewItem.Quantity = Quantity;
			Inventory.Add(ItemRowName, NewItem);

			UE_LOG(LogTemp, Warning, TEXT("Added new item with RowName %s, Quantity : %d"), *ItemRowName.ToString(), Quantity);
		}
		//// DataTable에서 해당 ID의 아이템을 로드하여 추가
		//FInventoryItem NewItem;
		//NewItem.ItemData.ItemID = ItemID;
		//NewItem.Quantity = Quantity;
		//Inventory.Add(ItemID, NewItem);

		//UE_LOG(LogTemp, Warning, TEXT("Added new item with ID %d, Quantity : %d"), ItemID, Quantity);
	}
}

void UInventoryActorComponent::RemoveItemFromInventory(FName ItemRowName, int32 Quantity)
{
	// 삭제할 아이템 인벤토리에 존재하는지 확인
	if (Inventory.Contains(ItemRowName))
	{
		FInventoryItem& ItemData = Inventory[ItemRowName];

		// 아이템의 수량을 감소시키고 0 이하일 경우 제거
		ItemData.Quantity -= Quantity;

		if (ItemData.Quantity <= 0)
		{
			Inventory.Remove(ItemRowName);
			UE_LOG(LogTemp, Warning,TEXT("Removed item with RowName %s from inventory"), *ItemRowName.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning,TEXT("Decrease quantity of item with RowName %s to %d."), *ItemRowName.ToString(), ItemData.Quantity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item with RowName %s not found in inventory."), *ItemRowName.ToString());
	}
}

bool UInventoryActorComponent::IsInventoryFull() const
{
	return Inventory.Num() >= MaxInventorySize;
}


