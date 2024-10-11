// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "NPC.generated.h"

UCLASS()
class MAGICIANS_API ANPC : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> ItemShopWidgetClass;

	UPROPERTY()
	UUserWidget* ItemShopWidget;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UCameraComponent* CameraComponent;	// 상호작용하면 Focus

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class USkeletalMeshComponent* Skeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UWidgetComponent* InteractionWidgetComponent;

public:
	virtual void DisplayInteractionWidget() override;

	virtual void HideInteractionWidget() override;

	virtual void Interact() override;

public:


};
