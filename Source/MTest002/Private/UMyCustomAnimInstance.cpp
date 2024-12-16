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
    // ���ø���ĳ�ʼ������
    Super::NativeInitializeAnimation();

    // ��ȡӵ���߽�ɫ
    OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
    if (OwningCharacter)
    {
        // �����ɫ�� MovementComponent
        MovementComponent = OwningCharacter->GetCharacterMovement();
    }

    // ��ʼ��
    CurrentSequence = nullptr;
   
}

void UMyCustomAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (!OwningCharacter || !MovementComponent)
    {
        return;
    }

    // �����ٶ�
    FVector Velocity = MovementComponent->Velocity;
    //GroundSpeedT = FVector::VectorPlaneProject(Velocity, FVector::UpVector).Size();
    GroundSpeedT = 500;

    // ��ȡ��ǰ���ٶ�
    CurrentAccelerationT = MovementComponent->GetCurrentAcceleration();

    // ����Ƿ�Ӧ���ƶ�
    bool HasSignificantAcceleration = !FMath::IsNearlyZero(CurrentAccelerationT.X, 0.01f) ||
        !FMath::IsNearlyZero(CurrentAccelerationT.Y, 0.01f) ||
        !FMath::IsNearlyZero(CurrentAccelerationT.Z, 0.01f);

    ShouldMoveT = GroundSpeedT > 3.0f && HasSignificantAcceleration;

    // �����Ƿ��ڿ���״̬
    IsFallingT = MovementComponent->IsFalling();

    // ���Ի�ȡ��ǰ���ŵĶ�������
    //USkeletalMeshComponent* SkeletalMeshComp = OwningCharacter->GetMesh();
    //if (SkeletalMeshComp)
    //{
    //    CurrentAnimSequence = GetPlayingAnimSequence(SkeletalMeshComp);
    //}

	//TestAnimNotifies(Cast<USkeletalMeshComponent>(OwningCharacter->GetMesh()));

    // ��ӡ��־����ѡ�����ڵ��ԣ�
    //UE_LOG(LogTemp, Warning, TEXT("GroundSpeed: %f, ShouldMove: %d, IsFalling: %d, Acceleration: %s"),
    //    GroundSpeedT, ShouldMoveT, IsFallingT, *CurrentAccelerationT.ToString());

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
    //    FString::Printf(TEXT("Speed: %.2f m/s, Acceleration: %s"),
    //        GroundSpeedT, *CurrentAccelerationT.ToString()));

        // ���µ�ǰ��������


}


UAnimSequence* UMyCustomAnimInstance::GetPlayingAnimSequence(USkeletalMeshComponent* SkeletalMeshComp)
{
    if (!SkeletalMeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComp is null!"));
        return nullptr;
    }

    // �������ģʽ�� AnimationBlueprint
    if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
    {
        // ������ͼģʽ��Ҫ�ض��Ĳ���ȥ��ȡ��ǰ���ŵ�����
        UE_LOG(LogTemp, Log, TEXT("Animation mode is AnimationBlueprint. Currently no direct way to access playing sequence."));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Animation mode is AnimationBlueprint. Currently no direct way to access playing sequence."));
        return nullptr;
    }

    // �������ģʽ�ǵ��ڵ�ģʽ
    if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationSingleNode)
    {
        return Cast<UAnimSequence>(SkeletalMeshComp->GetSingleNodeInstance()->GetCurrentAsset());
    }

    // �޷�ʶ��Ķ���ģʽ
    UE_LOG(LogTemp, Warning, TEXT("Unknown animation mode."));
    return nullptr;
    //else if (SkeletalMeshComp->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
    //{
    //    // ����󶨶�����ͼ�����
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
