// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "NPCShop.generated.h"

/**
 * 
 */
UCLASS()
class MAGICIANS_API ANPCShop : public ANPC
{
	GENERATED_BODY()
	
public:
		// ���� ������ �Ǹ� ��� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shop")
	TArray<FName> ItemOnSale;
};
