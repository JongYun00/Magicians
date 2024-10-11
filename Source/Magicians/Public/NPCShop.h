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
		// 상점 아이템 판매 취급 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shop")
	TArray<FName> ItemOnSale;
};
