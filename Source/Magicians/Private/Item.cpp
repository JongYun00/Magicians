// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "TPSPlayer.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh Component �ʱ�ȭ
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
		// �κ��丮�� ������ �߰� ����
		//Player->AddItemToInventory(ItemData);
		
		//Player->AddItemToInventory();

		Destroy(); // ���� �� ���忡�� ����
	}
}

void AItem::OnDrop(class ATPSPlayer* Player)
{
	if (Player)
	{
		// �κ��丮���� ���� �� ���忡 ������ ����
		FVector DropLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 100.f;
		AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(GetClass(), DropLocation, FRotator::ZeroRotator);
		DroppedItem->ItemData = ItemData;
		// ������ ����
		//Player->RemoveItemFromInventory();
	}
}

