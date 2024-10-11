// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ItemData.h"
#include "TPSPlayer.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MAGICIANS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class APBullet> magazine;

public:
	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* playerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookUpIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireIA;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractionIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InventoryIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Options")
	bool isinvertLookUp = false;

	UPROPERTY(VisibleAnywhere, Category = "EquipItem")
	class UStaticMeshComponent* WeaponMeshComp;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackAnimMontage;


public:

	UPROPERTY(EditAnywhere)
	float fireCurrentTime; // 타이머의 시간

	UPROPERTY(EditAnywhere)
	bool fireReady;		// 타이머 도달 여부

	UPROPERTY(EditAnywhere)
	float fireCoolTime = 1.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int64 CurrentMoney = 9999;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	class UInventoryActorComponent* DefaultInventory;
	//public:
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	//	TArray<FItemData> Inventory;
	//
	//public:
	//	UFUNCTION(BlueprintCallable, Category = "Invetory")
	//	void AddItemToInventory(const FItemData& item);
	//
	//	UFUNCTION(BlueprintCallable, Category = "Inventory")
	//	void RemoveItemFromInventory(int32 ItemID);

public:
	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void LookUp(const FInputActionValue& value);

	UFUNCTION()
	void Turn(const FInputActionValue& value);

	UFUNCTION()
	void InputJump();

	UFUNCTION()
	void InputFire();

	UFUNCTION()
	void InteractionPositive(const FInputActionValue& value);

	UFUNCTION()
	void GoInvetory();

	UFUNCTION()
	void SpawnBullet();

	void FireCoolTimer(float cooltime, float deltaTime);
private:
	FVector dir;

private:
	AActor* CachedInteractableActor; // 현재 상호작용 중인 액터를 저장


private:
	void PerformInteractionTrace();

public:

	UFUNCTION(BlueprintCallable)
	void UpdateMoney(int64 inputVal);

	void AddItemToInventory();

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerInventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UUserWidget* PlayerInventoryWidget;	// 실제 생성된 위젯을 저장하는 변수
	bool bIsInvetoryVisible; // 인벤토리의 현재 표시 상태를 저장하는 변수

	void ToggleInventory();
};
