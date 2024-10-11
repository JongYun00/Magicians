// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"

UCLASS()
class MAGICIANS_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	// 아이템 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* ItemMeshComponent;


public:
	// 아이템을 습득
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnPickup(class ATPSPlayer* Player);

	// 아이템을 버림
	UFUNCTION(BlueprintCallable, Category = "Item")
	void OnDrop(class ATPSPlayer* Player);



};
