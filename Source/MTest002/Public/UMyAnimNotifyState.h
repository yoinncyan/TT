// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UMyAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MTEST002_API UUMyAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyAnimNotifyTriggered);
	UPROPERTY(BlueprintAssignable)
	FOnMyAnimNotifyTriggered MyAnimNotifyStateBegin;
	FOnMyAnimNotifyTriggered MyAnimNotifyStateTick;
	FOnMyAnimNotifyTriggered MyAnimNotifyStateEnd;
};
