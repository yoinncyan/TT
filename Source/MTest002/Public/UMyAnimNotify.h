// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UMyAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class MTEST002_API UUMyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UUMyAnimNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyAnimNotifyTriggered);
	UPROPERTY(BlueprintAssignable)
	FOnMyAnimNotifyTriggered MyAnimNotifyTriggered;
};
