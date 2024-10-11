// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyStateFire.h"
#include "TPSPlayer.h"
#include "PBullet.h"

void UNotifyStateFire::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NotifyBegin"));

	tpsPlayer = Cast<ATPSPlayer>(MeshComp->GetOwner());
	if (tpsPlayer != nullptr)
	{
		tpsPlayer->SpawnBullet();
	}
}

void UNotifyStateFire::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NotifyTick"));
}

void UNotifyStateFire::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("NotifyEnd"));
}
