// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PBullet.h"
#include "InteractableInterface.h"
#include "InventoryActorComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ���̷�Ż �޽� �ʱ�ȭ
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CustomContents/Asset/UnityChanForUE/UnityChanForUE/unitychan.unitychan'"));

	if (InitMesh.Succeeded())	// ����� ������Ʈ�� �����Դٸ� ~ (���� + ��������)
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		// Relative -> ����� ��ġ��, ȸ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// sprintArm ���� - �ʱ�ȭ
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 530;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component �ʱ�ȭ
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// static Mesh �ʱ�ȭ
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	// ĳ���� �޽��� ���� + ��Ĺ ����
	// FName -> ���ڿ� -> �˻� ��ɿ� ����ȭ
	WeaponMeshComp->SetupAttachment(GetMesh(), FName("Character1_RightHandSocket"));

	DefaultInventory = CreateDefaultSubobject<UInventoryActorComponent>(TEXT("DefaultInventory"));
	DefaultInventory->MaxInventorySize = 30;


	fireCurrentTime = 0.f;
	fireCoolTime = 1.85f;
	fireReady = true;	// �߻� �غ� on
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(playerMappingContext, 0);
		}

		if (!PlayerInventoryWidget && PlayerInventoryWidgetClass)
		{
			PlayerInventoryWidget = CreateWidget<UUserWidget>(pc, PlayerInventoryWidgetClass);
			if (PlayerInventoryWidget != nullptr)
			{
				PlayerInventoryWidget->AddToViewport();	// ȭ�鿡 �߰�
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);	// �⺻������ ���� ó��
				bIsInvetoryVisible = false;
			}
		}
	}

	 FTimerHandle TraceTimerHandle;
	 GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &ATPSPlayer::PerformInteractionTrace, 0.2f, true);

}

// Fuck you
// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireCoolTimer(fireCoolTime, DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);

		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);

		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);

		EnhancedInputComponent->BindAction(JumpIA, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);

		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Triggered, this, &ATPSPlayer::InputFire);
		
		EnhancedInputComponent->BindAction(InteractionIA, ETriggerEvent::Triggered, this, &ATPSPlayer::InteractionPositive);
		
		EnhancedInputComponent->BindAction(InventoryIA, ETriggerEvent::Started, this, &ATPSPlayer::GoInvetory);
	}
}

void ATPSPlayer::Move(const FInputActionValue& value)
{
	const FVector CurrentValue = value.Get<FVector>();

	if (Controller != nullptr)
	{
		// dir ���Ϳ� �Էµ� X, Y ���� �����Ѵ�.
		dir.Y = CurrentValue.X;	// �¿� ���� ��
		dir.X = CurrentValue.Y;	// ���� ���� ��
	}

	// ī�޶��� ���� ȸ���� ���缭, MoveDirection�� ��ȯ
	dir = FTransform(GetControlRotation()).TransformVector(dir);

	AddMovementInput(dir);

	dir = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& value)
{
	// �Է� ���� float ���·� �����´�.
	// const float CurrentValue = value.Get<float>();
	// �Է� ���� float ���·� �����´�.
	float currentvalue;


	if (isinvertLookUp)
	{
		currentvalue = value.Get<float>() * -1.f;
	}
	else
	{
		currentvalue = value.Get<float>();
	}

	// ī�޶��� Pitch ���� �����Ͽ� ���Ʒ� ���� �̵��� �����Ѵ�.
	AddControllerPitchInput(currentvalue);
}

void ATPSPlayer::Turn(const FInputActionValue& value)
{
	// �Է� ���� float ���·� �����´�.
	const float CurrentValue = value.Get<float>();

	// ī�޶��� Yaw ���� �����Ͽ� �¿� ���� �̵��� ����
	AddControllerYawInput(CurrentValue);
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::InputFire()
{
	if (fireReady == true)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Yes"));
			AnimInstance->Montage_Play(AttackAnimMontage);
		}
		fireReady = false;
	}
}

void ATPSPlayer::InteractionPositive(const FInputActionValue& value)
{
	if (CachedInteractableActor != nullptr)
	{
		IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(CachedInteractableActor);
		// ��ȣ�ۿ� ����
		if (InteractableActor != nullptr)
		{
			InteractableActor->Interact();
		}
	}
}

void ATPSPlayer::GoInvetory()
{
	
}

void ATPSPlayer::SpawnBullet()
{
	FTransform firePosition = WeaponMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<APBullet>(magazine, firePosition);
}

void ATPSPlayer::FireCoolTimer(float cooltime, float deltaTime)
{
	if (fireCurrentTime < cooltime)
	{
		fireCurrentTime += deltaTime;
	}
	else
	{
		fireCurrentTime = 0;
		fireReady = true;
	}
}

void ATPSPlayer::PerformInteractionTrace()
{
	FVector _Start = GetActorLocation();
	FVector _End = GetActorLocation() + GetActorForwardVector() * 2000.f;

	FHitResult _HitOut;

	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel2, _TraceParams);

	// bHit -> Trace (����ü) ��� �� ������
	if (bHit)
	{
		AActor* HitActor = _HitOut.GetActor();

		if (HitActor != nullptr)
		{
			// �ش� ���Ϳ�, �������̽��� �����ϰ� �ִ��� ���θ� �˰� ������, Casting�� �غ��� �ȴ�.
			// Cast ���� ��, �ش� �������̽� ���� <-> �����ϸ�, ����
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitActor);

			if (InteractableActor != nullptr)
			{
				// ���ο� ��ȣ�ۿ� ����� ���� ��ȣ�ۿ� ���� �ٸ���
				if (CachedInteractableActor != HitActor)
				{
					// ���� ��ȣ�ۿ� ����� �־��ٸ� ������ ����
					if (CachedInteractableActor != nullptr)
					{
						IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
						if (CachedInteractable)
						{
							CachedInteractable->HideInteractionWidget();
						}
					}

					// ���ο� ���͸� ĳ���ϰ� ���� ǥ��
					CachedInteractableActor = HitActor;
					InteractableActor->DisplayInteractionWidget();
				}

			}
		}
	}

	else
	{
		if (CachedInteractableActor != nullptr)
		{
			IInteractableInterface* CacheInteractable = Cast<IInteractableInterface>(CachedInteractableActor);

			if (CacheInteractable != nullptr)
			{
				CacheInteractable->HideInteractionWidget();
			}
			CachedInteractableActor = nullptr;
		}
	}

	if (bHit)
	{
		DrawDebugLine(GetWorld(), _Start, _HitOut.ImpactPoint, FColor::Red, false, 5.f, 0.f, 2.f);
		DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.f, 12.f, FColor::Yellow, false, 5.f);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Actor: %s"), *_HitOut.GetActor()->GetName()));
	}

	else
	{
		DrawDebugLine(GetWorld(), _Start, _End, FColor::Blue, false, 5.f, 0, 2.f);
		// LineTrace ���� �� ����� �޽��� ���
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Hit"));
	}
}

//void ATPSPlayer::AddItemToInventory(const FItemData& item)
//{
//	for (auto inventory : Inventory)
//	{
//		if (inventory == item)
//		{
//			// ���� ����
//			
//			return;
//		}
//	}
//	
//	Inventory.Add(item);
//}



void ATPSPlayer::AddItemToInventory()
{

}

void ATPSPlayer::ToggleInventory()
{
	APlayerController* pc = Cast<APlayerController>(GetController());

	if (pc != nullptr)
	{
		if (PlayerInventoryWidget != nullptr)
		{
			if (bIsInvetoryVisible)
			{
				// �κ��丮�� �����ִٸ� �����
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
				bIsInvetoryVisible = false;

				// �Է� ��� ����
				FInputModeGameOnly InputMode;
				pc->SetInputMode(InputMode);
				pc->bShowMouseCursor = false;
			}
		}
	}
}

//void ATPSPlayer::RemoveItemFromInventory(int32 ItemID)
//{
//	for (int32 i = 0; i < Inventory.Num(); ++i)
//	{
//		// ������ ID�� ��ġ�ϴ��� Ȯ��
//		if (Inventory[i].ItemID == ItemID)
//		{
//			// ���� ������ ���������� Ȯ��
//			if (Inventory[i].bIsStackable)
//			{
//				// ������ 1����
//				Inventory[i].MaxStackCount -= 1;
//
//				// ������ 0���ϰ� �Ǹ� �κ��丮���� ����
//				if (Inventory[i].MaxStackCount <= 0)
//				{
//					Inventory.RemoveAt(i);
//					UE_LOG(LogTemp, Warning, TEXT("Item removed : %d"), ItemID);
//				}
//				else
//				{
//					UE_LOG(LogTemp, Warning, TEXT("Item quantity decreased : %d, Remaining : %d"), ItemID, Inventory[i].MaxStackCount);
//				}
//			}
//			else
//			{
//				Inventory.RemoveAt(i);
//				UE_LOG(LogTemp, Warning, TEXT("Non-stackable item reomved : %d"), ItemID);
//			}
//		}
//
//		// �������� ó���� �� �Լ��� ����
//		return;
//	}
//	UE_LOG(LogTemp, Warning, TEXT("Item not found in inventory : %d"), ItemID);
//}
//
//void ATPSPlayer::RemoveItemFromInventory()
//{
//
//}

void ATPSPlayer::UpdateMoney(int64 inputVal)
{
	int64 _result;
	_result = CurrentMoney + inputVal;
	
	if (_result < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Not Enough Money !"));
	}
	else
	{
		CurrentMoney = _result;
	}
}

