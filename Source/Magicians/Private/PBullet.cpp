// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("MainCollidor"));
	SphereCollComp->SetCollisionProfileName(TEXT("BlockAll"));
	SphereCollComp->SetSphereRadius(13);
	SetRootComponent(SphereCollComp);

	SMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SMeshComp->SetupAttachment(SphereCollComp);
	SMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �߻�ü ������Ʈ ����
	PMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMovementComp"));

	// �߻�ü ������Ʈ�� Update ��ų ���(������Ʈ) ����
	PMovementComp->SetUpdatedComponent(SphereCollComp);

	// �ʱ� �ӵ�
	PMovementComp->InitialSpeed = 3000.f;

	// �ִ� �ӵ�
	PMovementComp->MaxSpeed = 5000.f;

	// �ݵ�
	PMovementComp->bShouldBounce = true;

	// �ݵ� ũ��
	PMovementComp->Bounciness = 0.3f;
	
	// ������� (����, ��)
	//InitialLifeSpan = 2.0f;
	
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;	// �����带 ������ �Ҵ�
	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([this]()->void{Destroy();}), 2.f, false);
	// (�˸�, �˸�ó���� �� ��ü, �˸� ó�� �Լ�, �˸� �ð�, �ݺ� ����, ȣ�� �� ������ �ð�)
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APBullet::Die()
{
	Destroy();
}

