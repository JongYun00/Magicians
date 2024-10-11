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

	// 발사체 컴포넌트 생성
	PMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMovementComp"));

	// 발사체 컴포넌트를 Update 시킬 대상(컴포넌트) 지정
	PMovementComp->SetUpdatedComponent(SphereCollComp);

	// 초기 속도
	PMovementComp->InitialSpeed = 3000.f;

	// 최대 속도
	PMovementComp->MaxSpeed = 5000.f;

	// 반동
	PMovementComp->bShouldBounce = true;

	// 반동 크기
	PMovementComp->Bounciness = 0.3f;
	
	// 생명길이 (단위, 초)
	//InitialLifeSpan = 2.0f;
	
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;	// 쓰레드를 독립적 할당
	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([this]()->void{Destroy();}), 2.f, false);
	// (알림, 알림처리를 할 객체, 알림 처리 함수, 알림 시간, 반복 여부, 호출 전 딜레이 시간)
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

