// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class MAGICIANS_API UItemManager : public UObject
{
	GENERATED_BODY()
	
	// ������ �����͸� �����ϴ� �� (ItemId -> FItemData)
	TMap<int32, FItemData> ItemDataMap;

	// Data ���̺��� ������ �����͸� �ε�
	UFUNCTION(BlueprintCallable, Category = "Item")
	void LoadItemData(UDataTable* ItemDataTable);

	// ItemID�� ������ ������ �˻�
	/*UFUNCTION(BlueprintCallable, Category = "Item")
	TOptional<FItemData> GetItemDataByIDOptional(int32 ItemID);*/

	// ItemID�� ������ ������ �˻�
	//UFUNCTION(BlueprintCallable, Category = "Item")
	//FItemData& GetItemDataByID(int32 ItemID);
};
