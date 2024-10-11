// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 *
 */

 UENUM(BlueprintType)
	 enum class EItemType : uint8
 {
	 Consumable, // �Ҹ� ������
	 Equipment, // ���� ������ ������
	 Quest, // ����Ʈ ������
	 Material // ��� ������
 };

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	bool operator== (const FItemData& item) const
	{
		return this->ItemID == item.ItemID;
	}

	// ������ ���� ID (DataTable ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;
	
	// The type of the item: Equipment, Consumable, Quest, Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	// Name of the Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	// Description of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	// 2D thumbnail for inventory or shop display
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemThumbnail;

	// Mesh to represent the item in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* ItemMesh;

	// Item's base price in the shop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Price;

	// Whether the item is stackable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;

	// Max stack count (if applicable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackCount;	// ����� �̹���

};

class MAGICIANS_API ItemData
{
public:
	ItemData();
	~ItemData();
};
