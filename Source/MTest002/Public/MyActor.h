// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class MTEST002_API AMyActor : public AActor
{
	GENERATED_BODY()

//BlueprintReadOnly 是一个 UE 的宏，用于允许蓝图访问 C++ 类中的属性，但它只能用于 protected 或 public 修饰的成员，不能用于 private 成员。
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Components")
	
	UStaticMeshComponent* MeshComponent;

public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//public:
//	UFUNCTION(BlueprintCallable, Category = "MeshControl")
//	void ChangeMeshColor(FLinearColor NewColor);
};
