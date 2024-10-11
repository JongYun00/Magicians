// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values

ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("InteractionBox");
	SetRootComponent(BoxComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("FocusCamera");
	CameraComponent->SetupAttachment(BoxComponent);
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal");
	Skeletal->SetupAttachment(BoxComponent);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	InteractionWidgetComponent->SetupAttachment(Skeletal);

	InteractionWidgetComponent->SetRelativeLocation(FVector(0, 0, 220.f));
	InteractionWidgetComponent->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	InteractionWidgetComponent->SetDrawSize(FVector2D(59.f, 50.f));

	if (InteractionWidgetClass != nullptr)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
	}
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	// 위젯을 비활성화 상태로 시작
	InteractionWidgetComponent->SetVisibility(false);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::DisplayInteractionWidget()
{
	if (InteractionWidgetComponent != nullptr)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void ANPC::HideInteractionWidget()
{
	if (InteractionWidgetComponent != nullptr)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}

void ANPC::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interacted with object."));

	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);

	pc->SetViewTargetWithBlend(this, 1.f);
	pc->SetInputMode(FInputModeUIOnly());
	pc->SetShowMouseCursor(true);

	HideInteractionWidget();

	if (ItemShopWidgetClass != nullptr)
	{
		ItemShopWidget = CreateWidget<UUserWidget>(GetWorld(), ItemShopWidgetClass);
		
		FName PropertyName(TEXT("OwnerNPC"));
		FProperty* _npcProperty = ItemShopWidget->GetClass()->FindPropertyByName(PropertyName);
		if (_npcProperty != nullptr)
		{
			FObjectProperty* ObjectProperty = CastField<FObjectProperty>(_npcProperty);
			if (ObjectProperty && ObjectProperty->PropertyClass == ANPC::StaticClass())
			{
				ObjectProperty->SetObjectPropertyValue_InContainer(ItemShopWidget, this);
			}
		}

		if (ItemShopWidget != nullptr)
		{
			ItemShopWidget->AddToViewport();
		}
	}
}

