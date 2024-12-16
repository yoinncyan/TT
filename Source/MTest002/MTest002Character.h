// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UMyAnimNotify.h"
#include "UMyAnimNotifyState.h"
#include "MTest002PlayerController.h"
#include "InputActionValue.h"
#include "MTest002Character.generated.h"

class UInputMappingContext; // 声明类以便在头文件中使用
class UInputAction; // 声明 UInputAction 类

UCLASS(Blueprintable)
class AMTest002Character : public ACharacter
{
	GENERATED_BODY()

public:
	AMTest002Character();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Input Actions and Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MouseWheelAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Function to adjust the camera height */
	void AdjustCameraHeight(const FInputActionValue& Value);


	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAnimNotifyTriggered(); // 用于处理动画通知的函数

	UFUNCTION()
	void OnAnimNotifyStateBegin(); // 用于处理动画通知状态开始的函数

	UFUNCTION()
	void OnAnimNotifyStateTick(); // 用于处理动画通知状态更新的函数

	UFUNCTION()
	void OnAnimNotifyStateEnd(); // 用于处理动画通知状态结束的函数

	/** Camera height configuration */
	float CameraBoomMinLength = 200.f; // 相机臂最小长度
	float CameraBoomMaxLength = 3500.f; // 相机臂最大长度
	float CameraZoomSpeed = 300.f; // 相机缩放速度

};

