// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyCustomAnimInstance.h"
#include "Engine/Engine.h"
#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_SequencePlayer.h"
#include "Animation/AnimBlueprintGeneratedClass.h"


void UMyCustomAnimInstance::NativeInitializeAnimation()
{
    // 调用父类的初始化方法
    Super::NativeInitializeAnimation();

    // 获取拥有者角色
    OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (OwningCharacter)
    {
        // 缓存角色的 MovementComponent
        MovementComponent = OwningCharacter->GetCharacterMovement();
    }

    // 初始化
    CurrentSequence = nullptr;
   
}

void UMyCustomAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (!OwningCharacter || !MovementComponent)
    {
        return;
    }

    // 更新速度
    FVector Velocity = MovementComponent->Velocity;
    //GroundSpeedT = FVector::VectorPlaneProject(Velocity, FVector::UpVector).Size();
    GroundSpeedT = 500;

    // 获取当前加速度
    CurrentAccelerationT = MovementComponent->GetCurrentAcceleration();

    // 检查是否应该移动
    bool HasSignificantAcceleration = !FMath::IsNearlyZero(CurrentAccelerationT.X, 0.01f) ||
        !FMath::IsNearlyZero(CurrentAccelerationT.Y, 0.01f) ||
        !FMath::IsNearlyZero(CurrentAccelerationT.Z, 0.01f);

    ShouldMoveT = GroundSpeedT > 3.0f && HasSignificantAcceleration;

    // 更新是否处于空中状态
    IsFallingT = MovementComponent->IsFalling();

    // 尝试获取当前播放的动画序列
    //USkeletalMeshComponent* SkeletalMeshComp = OwningCharacter->GetMesh();
    //if (SkeletalMeshComp)
    //{
    //    CurrentAnimSequence = GetPlayingAnimSequence(SkeletalMeshComp);
    //}

	//TestAnimNotifies(Cast<USkeletalMeshComponent>(OwningCharacter->GetMesh()));

    // 打印日志（可选，用于调试）
    //UE_LOG(LogTemp, Warning, TEXT("GroundSpeed: %f, ShouldMove: %d, IsFalling: %d, Acceleration: %s"),
    //    GroundSpeedT, ShouldMoveT, IsFallingT, *CurrentAccelerationT.ToString());

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
    //    FString::Printf(TEXT("Speed: %.2f m/s, Acceleration: %s"),
    //        GroundSpeedT, *CurrentAccelerationT.ToString()));

        // 更新当前动画序列


}


UAnimSequence* UMyCustomAnimInstance::GetPlayingAnimSequence(USkeletalMeshComponent* SkeletalMeshComp)
{
    if (!SkeletalMeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComp is null!"));
        return nullptr;
    }

    // 如果动画模式是 AnimationBlueprint
    if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
    {
        // 动画蓝图模式需要特定的操作去获取当前播放的序列
        UE_LOG(LogTemp, Log, TEXT("Animation mode is AnimationBlueprint. Currently no direct way to access playing sequence."));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Animation mode is AnimationBlueprint. Currently no direct way to access playing sequence."));
        return nullptr;
    }

    // 如果动画模式是单节点模式
    if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationSingleNode)
    {
        return Cast<UAnimSequence>(SkeletalMeshComp->GetSingleNodeInstance()->GetCurrentAsset());
    }

    // 无法识别的动画模式
    UE_LOG(LogTemp, Warning, TEXT("Unknown animation mode."));
    return nullptr;
    //else if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
    //{
    //    // 处理绑定动画蓝图的情况
    //    return GetPlayingAnimSequenceFromBlueprint(SkeletalMeshComp);
    //}

    //UE_LOG(LogTemp, Warning, TEXT("Animation mode is not SingleNode."));
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Animation mode is not SingleNode."));
}




//MyCustomAnimInstance::MyCustomAnimInstance()
//{
//}
//
//MyCustomAnimInstance::~MyCustomAnimInstance()
//{
//}
