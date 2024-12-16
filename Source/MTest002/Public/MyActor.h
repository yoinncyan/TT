// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class MTEST002_API AMyActor : public AActor
{
	GENERATED_BODY()

//BlueprintReadOnly ��һ�� UE �ĺ꣬����������ͼ���� C++ ���е����ԣ�����ֻ������ protected �� public ���εĳ�Ա���������� private ��Ա��
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
