// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "TPSPlayer.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh Component 초기화
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMeshComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnPickup(class ATPSPlayer* Player)
{
	if (Player)
	{
		// 인벤토리에 아이템 추가 로직
		//Player->AddItemToInventory(ItemData);
		
		//Player->AddItemToInventory();

		Destroy(); // 습득 후 월드에서 삭제
	}
}

void AItem::OnDrop(class ATPSPlayer* Player)
{
	if (Player)
	{
		// 인벤토리에서 제거 및 월드에 아이템 생성
		FVector DropLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 100.f;
		AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(GetClass(), DropLocation, FRotator::ZeroRotator);
		DroppedItem->ItemData = ItemData;
		// 아이템 삭제
		//Player->RemoveItemFromInventory();
	}
}

