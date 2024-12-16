// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyAnimNotifyState.h"

void UUMyAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	MyAnimNotifyStateBegin.Broadcast();

}

void UUMyAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	MyAnimNotifyStateTick.Broadcast();
}

void UUMyAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	MyAnimNotifyStateEnd.Broadcast();
}

FString UUMyAnimNotifyState::GetNotifyName_Implementation() const
{
	return FString("UMyAnimNotifyState");
}
