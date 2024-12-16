// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Engine/Engine.h"
#include "ResourceManager.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ����̬�������
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	// ȷ��RootComponentû�б��ظ�����
	if (!RootComponent)
	{
		RootComponent = MeshComponent;
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello,Unreal Engine C++!"));
		//CreateMeshComponent();
		//ChangeMeshColor(FLinearColor::Red);

		//UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
				// ������Ч·��
		FString ValidPath = TEXT("/Engine/BasicShapes/Cube.Cube");
		// �첽������Դ
		ResourceManager::Get().LoadResourceAsync2<UStaticMesh, UStaticMeshComponent>(ValidPath, MeshComponent);


		//if (ValidMesh)
		//{
		//	MeshComponent->SetStaticMesh(ValidMesh);
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Failed to load static mesh asset or asset is null!"));
		//}
	}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}