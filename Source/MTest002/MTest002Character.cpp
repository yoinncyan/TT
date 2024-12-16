// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTest002Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AMTest002Character::AMTest002Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 初始化输入映射上下文和输入操作
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> PlayerMappingContextFinder(TEXT("/Game/TopDown/Input/IMC_Default.IMC_Default"));
	if (PlayerMappingContextFinder.Succeeded())
	{
		InputMappingContext = PlayerMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseWheelActionFinder(TEXT("/Game/TopDown/Input/Actions/IA_MouseWheel.IA_MouseWheel"));
	if (MouseWheelActionFinder.Succeeded())
	{
		MouseWheelAction = MouseWheelActionFinder.Object;
	}

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMTest002Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}



void AMTest002Character::BeginPlay()
{
	Super::BeginPlay();


	if (InputMappingContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputMappingContext is valid."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputMappingContext is NOT initialized properly."));
	}

	// 绑定到 OnMyAnimNotifyTriggered 事件
	//UUMyAnimNotify::OnMyAnimNotifyTriggered.AddDynamic(this, &AMTest002Character::OnAnimNotifyTriggered);
	// 获取所有动画通知实例，并绑定委托
	TArray<UUMyAnimNotify*> Notifies;
	TArray<UUMyAnimNotifyState*> NotifyStates;
	for (TObjectIterator<UUMyAnimNotify> It; It; ++It)
	{
		UUMyAnimNotify* NotifyInstance = *It;
		if (NotifyInstance)
		{
			NotifyInstance->MyAnimNotifyTriggered.AddDynamic(this, &AMTest002Character::OnAnimNotifyTriggered);
		}
	}

	for (TObjectIterator<UUMyAnimNotifyState> It; It; ++It)
	{
		UUMyAnimNotifyState* NotifyStateInstance = *It;
		if (NotifyStateInstance)
		{
			NotifyStateInstance->MyAnimNotifyStateBegin.AddDynamic(this, &AMTest002Character::OnAnimNotifyStateBegin);
			NotifyStateInstance->MyAnimNotifyStateTick.AddDynamic(this, &AMTest002Character::OnAnimNotifyStateTick);
			NotifyStateInstance->MyAnimNotifyStateEnd.AddDynamic(this, &AMTest002Character::OnAnimNotifyStateEnd);
		}
	}

	// 获取玩家控制器并转换为 AMTest002PlayerController

	AMTest002PlayerController* PlayerController = Cast<AMTest002PlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		//PlayerController->SetInputMode(FInputModeGameOnly());
		//PlayerController->bShowMouseCursor = true; // 根据你的需求显示鼠标
		// 获取增强输入的本地子系统
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			// 添加输入映射上下文，确保优先级足够高
			Subsystem->AddMappingContext(InputMappingContext, 0); // 0 表示高优先级
		}
	}

}

void AMTest002Character::OnAnimNotifyTriggered()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UMyAnimNotify 被触发，在 Pawn 类中接收到通知(该通知用于右脚抬起时)"));
	}
}

void AMTest002Character::OnAnimNotifyStateBegin()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UMyAnimNotifyState 被触发，在 Pawn 类中接收到通知(该通知状态用于開始跳躍中的狀態)"));
	//}
}
void AMTest002Character::OnAnimNotifyStateTick()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UMyAnimNotifyState 被触发，在 Pawn 类中接收到通知(该通知状态用于正在處於跳躍落下的過程中)"));
	//}
}
void AMTest002Character::OnAnimNotifyStateEnd()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UMyAnimNotifyState 被触发，在 Pawn 类中接收到通知(该通知状态用于跳躍落下過程結束)"));
	//}
}

void AMTest002Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 确保输入组件是增强输入组件类型
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MouseWheelAction)
		{
			EnhancedInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &AMTest002Character::AdjustCameraHeight);
		}
	}
}


void AMTest002Character::AdjustCameraHeight(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("AdjustCameraHeight called, AxisValue: %f"), AxisValue));

	// 在滚轮滚动时持续调整相机高度
	if (AxisValue != 0.0f)
	{
		float NewLength = CameraBoom->TargetArmLength + AxisValue *10* CameraZoomSpeed * GetWorld()->GetDeltaSeconds();
		CameraBoom->TargetArmLength = FMath::Clamp(NewLength, CameraBoomMinLength, CameraBoomMaxLength);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("New CameraBoom Length: %f"), CameraBoom->TargetArmLength));
	}

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("AdjustCameraHeight called, AxisValue: %f"), AxisValue));
	//}

	//if (AxisValue != 0.0f)
	//{
	//	float NewTargetArmLength = CameraBoom->TargetArmLength + AxisValue * CameraZoomSpeed;

	//	// 调试信息：打印未限制前的相机臂长度
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("New Target Arm Length before clamp: %f"), NewTargetArmLength));
	//	}

	//	// 限制在最大和最小之间
	//	NewTargetArmLength = FMath::Clamp(NewTargetArmLength, CameraBoomMinLength, CameraBoomMaxLength);
	//	CameraBoom->TargetArmLength = NewTargetArmLength;

	//	// 调试信息：打印最终相机臂长度
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("New Camera Boom Length: %f"), NewTargetArmLength));
	//	}
	//}
}