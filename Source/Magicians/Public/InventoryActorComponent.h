// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryActorComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FInventoryItem() //�⺻ ���� ���� 1��
		:ItemData(), Quantity(1)
	{}
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class MAGICIANS_API UInventoryActorComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	UInventoryActorComponent();

public:
	//�������� ���� ID�� Ű�� ����ϴ� �κ��丮 ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, FInventoryItem> Inventory;

	// DataTable�� �������Ʈ���� �Ҵ��� �� �ֵ��� UPROPERTY ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;

public:
	//DataTable�κ��� �����۷ε�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FItemData LoadItemFromDataTable(FName ItemRowName);

	//�κ��丮�� ������ �߰�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(FName ItemRowName, int32 Quantity);

	//�κ��丮�� ������ ����
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventory(FName ItemRowName, int32 Quantity);
	//�κ��丮�� ���� á���� Ȯ��
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull() const;

public:

	//�κ��丮 �ִ� ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
