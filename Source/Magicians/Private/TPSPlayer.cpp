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


	// 스켈레탈 메쉬 초기화
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CustomContents/Asset/UnityChanForUE/UnityChanForUE/unitychan.unitychan'"));

	if (InitMesh.Succeeded())	// 제대로 오브젝트를 가져왔다면 ~ (접근 + 가져오기)
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		// Relative -> 상대적 위치값, 회전값
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	// sprintArm 생성 - 초기화
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 530;
	springArmComp->bUsePawnControlRotation = true;

	// Camera Component 초기화
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// static Mesh 초기화
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	// 캐릭터 메쉬에 부착 + 소캣 부착
	// FName -> 문자열 -> 검색 기능에 최적화
	WeaponMeshComp->SetupAttachment(GetMesh(), FName("Character1_RightHandSocket"));

	DefaultInventory = CreateDefaultSubobject<UInventoryActorComponent>(TEXT("DefaultInventory"));
	DefaultInventory->MaxInventorySize = 30;


	fireCurrentTime = 0.f;
	fireCoolTime = 1.85f;
	fireReady = true;	// 발사 준비 on
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
				PlayerInventoryWidget->AddToViewport();	// 화면에 추가
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);	// 기본적으로 숨김 처리
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
		// dir 벡터에 입력된 X, Y 값을 설정한다.
		dir.Y = CurrentValue.X;	// 좌우 방향 값
		dir.X = CurrentValue.Y;	// 전후 방향 값
	}

	// 카메라의 현재 회전에 맞춰서, MoveDirection을 변환
	dir = FTransform(GetControlRotation()).TransformVector(dir);

	AddMovementInput(dir);

	dir = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& value)
{
	// 입력 값을 float 형태로 가져온다.
	// const float CurrentValue = value.Get<float>();
	// 입력 값을 float 형태로 가져온다.
	float currentvalue;


	if (isinvertLookUp)
	{
		currentvalue = value.Get<float>() * -1.f;
	}
	else
	{
		currentvalue = value.Get<float>();
	}

	// 카메라의 Pitch 값을 변경하여 위아래 시점 이동을 수행한다.
	AddControllerPitchInput(currentvalue);
}

void ATPSPlayer::Turn(const FInputActionValue& value)
{
	// 입력 값을 float 형태로 가져온다.
	const float CurrentValue = value.Get<float>();

	// 카메라의 Yaw 값을 변경하여 좌우 시점 이동을 수행
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
		// 상호작용 수행
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

	// bHit -> Trace (구조체) 결과 값 있으면
	if (bHit)
	{
		AActor* HitActor = _HitOut.GetActor();

		if (HitActor != nullptr)
		{
			// 해당 액터에, 인터페이스를 소유하고 있는지 여부를 알고 싶으면, Casting을 해보면 된다.
			// Cast 성공 시, 해당 인터페이스 소유 <-> 실패하면, 없음
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(HitActor);

			if (InteractableActor != nullptr)
			{
				// 새로운 상호작용 대상이 이전 상호작용 대상과 다르면
				if (CachedInteractableActor != HitActor)
				{
					// 이전 상호작용 대상이 있었다면 위젯을 숨김
					if (CachedInteractableActor != nullptr)
					{
						IInteractableInterface* CachedInteractable = Cast<IInteractableInterface>(CachedInteractableActor);
						if (CachedInteractable)
						{
							CachedInteractable->HideInteractionWidget();
						}
					}

					// 새로운 액터를 캐시하고 위젯 표시
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
		// LineTrace 실패 시 디버그 메시지 출력
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Hit"));
	}
}

//void ATPSPlayer::AddItemToInventory(const FItemData& item)
//{
//	for (auto inventory : Inventory)
//	{
//		if (inventory == item)
//		{
//			// 증가 로직
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
				// 인벤토리가 열려있다면 숨기기
				PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
				bIsInvetoryVisible = false;

				// 입력 모드 복구
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
//		// 아이템 ID가 일치하는지 확인
//		if (Inventory[i].ItemID == ItemID)
//		{
//			// 스택 가능한 아이템인지 확인
//			if (Inventory[i].bIsStackable)
//			{
//				// 수량을 1감소
//				Inventory[i].MaxStackCount -= 1;
//
//				// 수령이 0이하가 되면 인벤토리에서 제거
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
//		// 아이템을 처리한 후 함수를 종료
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

