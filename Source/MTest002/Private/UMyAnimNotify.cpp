// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyAnimNotify.h"


UUMyAnimNotify::UUMyAnimNotify()
{

}

void UUMyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
    MyAnimNotifyTriggered.Broadcast(); // 广播通知触发事件
}
