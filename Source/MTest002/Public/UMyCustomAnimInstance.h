// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_SequencePlayer.h"
#include "Animation/BlendSpace.h"
#include "Animation/BlendSpace1D.h"
#include "UMyCustomAnimInstance.generated.h"


 /**
  * �Զ��嶯��ʵ����
  */
UCLASS()
class MTEST002_API UMyCustomAnimInstance : public UAnimInstance
{

	GENERATED_BODY()

public:
    // �����ɫ���˶����������
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    ACharacter* OwningCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    UCharacterMovementComponent* MovementComponent;

    // ��������
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    float GroundSpeedT;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool ShouldMoveT;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool IsFallingT;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    FVector CurrentAccelerationT; // ��ǰ���ٶ�

    /** ��ǰ���ŵĶ������� */
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    UAnimSequence* CurrentAnimSequence;

    // ���浱ǰ���ŵĶ�������
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    UAnimSequenceBase* CurrentSequence;

public:

	// ��ʼ��ʱ���߼�
	virtual void NativeInitializeAnimation() override;

	// ÿ֡�����߼�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



    UFUNCTION(BlueprintCallable, Category = "Animation")
    UAnimSequence* GetPlayingAnimSequence(USkeletalMeshComponent* SkeletalMeshComp);


    // ��ȡ��ǰʹ�õ��Ӷ���
    //UAnimSequence* GetCurrentSequenceFromBlendSpace(UBlendSpace* BlendSpace, const FVector& BlendInput);




	//MyCustomAnimInstance();
	//~MyCustomAnimInstance();
};
