// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPath.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

/**
 * 
 */
class MTEST002_API ResourceManager
{
public:

	// 获取单例实例
	static ResourceManager& Get();

	// 禁止拷贝和赋值
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	// 加载资源模板(同步加载资源)
	template <typename T>
	T* LoadResource(const FString& Path);

    // // 加载资源模板(异步加载资源)
    template <typename T>
    void LoadResourceAsync(const FString& Path, TFunction<void(T*)> OnLoadedCallback);

    // 加载资源模板2(异步加载资源)
    template <typename TResource, typename TComponent>
    void LoadResourceAsync2(const FString& Path, TComponent* TargetComponent);

    //内部处理逻辑
    template <typename TResource, typename TComponent>
    void OnResourceLoaded(TResource* LoadedResource, TComponent* TargetComponent);

	// 清空缓存
	void ClearCache();

	ResourceManager();
	~ResourceManager();

	// 缓存已加载的资源
	TMap<FString, UObject*> ResourceCache;

    // StreamableManager 用于异步加载
    FStreamableManager StreamableManager;
};

// 模板函数的实现必须在头文件中
// 同步加载资源
template <typename T>
T* ResourceManager::LoadResource(const FString& Path)
{
    // 检查缓存
    if (UObject** CachedResource = ResourceCache.Find(Path))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Loaded from cache: %s"), *Path));
        }
        return Cast<T>(*CachedResource);
    }

    // 动态加载资源
    T* LoadedResource = LoadObject<T>(nullptr, *Path);
    if (LoadedResource)
    {
        ResourceCache.Add(Path, LoadedResource);

        // 如果加载成功
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Successfully loaded resource: %s"), *Path));
            UE_LOG(LogTemp, Log, TEXT("加载资源成功！"));
        }

        return LoadedResource;
    }

    // 如果加载失败
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Failed to load resource: %s"), *Path));
        UE_LOG(LogTemp, Log, TEXT("加载资源失败！"));
    }

    UE_LOG(LogTemp, Warning, TEXT("Failed to load resource at path: %s"), *Path);
    return nullptr;
}

// 异步加载资源
template <typename T>
void ResourceManager::LoadResourceAsync(const FString& Path, TFunction<void(T*)> OnLoadedCallback)
{
    // **1. 检查缓存**
    // 如果资源路径已经存在于缓存中，直接返回缓存的资源，避免重复加载。
    if (UObject** CachedResource = ResourceCache.Find(Path))
    {
        if (GEngine)
        {
            // 在屏幕上显示加载缓存成功的消息（绿色提示）。
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Loaded from cache: %s"), *Path));
        }

        // 调用回调函数，返回缓存中的资源。
        OnLoadedCallback(Cast<T>(*CachedResource));
        return;
    }

    // 异步加载方式1
    // **2. 准备异步加载的资源路径**
    // 将传入的路径字符串转换为 FSoftObjectPath，用于异步加载。
    FSoftObjectPath SoftObjectPath(Path);

    // 获取 StreamableManager，用于管理异步加载任务。
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // **3. 请求异步加载**
    Streamable.RequestAsyncLoad(
        SoftObjectPath,/**要加载的资源路径**/
        FStreamableDelegate::CreateLambda([this, Path, OnLoadedCallback, SoftObjectPath]()
            {
                // **4. 解析加载结果**
                // 通过 SoftObjectPath.ResolveObject() 获取加载完成的资源。
                UObject* Object = SoftObjectPath.ResolveObject();
                if (Object)
                {
                    // 如果资源成功加载，将其添加到缓存中。
                    ResourceCache.Add(Path, Object);

                    // 在屏幕上显示加载成功的消息（绿色提示）。
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Successfully loaded resource asynchronously: %s"), *Path));
                    }

                    // 调用回调函数，传递加载的资源。
                    OnLoadedCallback(Cast<T>(Object));
                }
                else
                {
                    // 如果加载失败，在屏幕上显示加载失败的消息（红色提示）。
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Failed to load resource asynchronously: %s"), *Path));
                    }

                    // 在日志中记录加载失败的信息。
                    UE_LOG(LogTemp, Warning, TEXT("Failed to load resource asynchronously at path: %s"), *Path);

					// 调用回调函数，传递空指针。
                    OnLoadedCallback(nullptr);
                }
            })
    );
}

// 异步加载资源并将其应用到目标组件
template <typename TResource, typename TComponent>
void ResourceManager::LoadResourceAsync2(const FString& Path, TComponent* TargetComponent)
{
    // 检查目标组件是否有效
    if (!TargetComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetComponent is null!"));
        return;
    }

    // 检查是否已经在缓存中存在资源
    if (UObject** CachedResource = ResourceCache.Find(Path))
    {
        // 从缓存中获取资源并直接调用加载完成的逻辑
        TResource* Resource = Cast<TResource>(*CachedResource);
        OnResourceLoaded(Resource, TargetComponent);
        return;
    }

    // 创建资源路径对象
    FSoftObjectPath SoftObjectPath(Path);

    // 获取全局的 StreamableManager，用于异步加载资源
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // 请求异步加载资源
    Streamable.RequestAsyncLoad(
        SoftObjectPath,
        FStreamableDelegate::CreateLambda([this, Path, SoftObjectPath, TargetComponent]()
            {
                // 异步加载完成后的回调函数
                UObject* Object = SoftObjectPath.ResolveObject();
                if (Object)
                {
                    // 成功加载资源，将其添加到缓存中
                    ResourceCache.Add(Path, Object);

                    // 将资源转换为指定类型并调用加载完成逻辑
                    TResource* Resource = Cast<TResource>(Object);
                    OnResourceLoaded(Resource, TargetComponent);
                }
                else
                {
                    // 如果加载失败，记录错误日志并调用加载失败逻辑
                    UE_LOG(LogTemp, Warning, TEXT("Failed to load resource asynchronously: %s"), *Path);
                    OnResourceLoaded<TResource, TComponent>(nullptr, TargetComponent);
                }
            })
    );
}

// 内部处理逻辑：处理加载完成后的操作
template <typename TResource, typename TComponent>
void ResourceManager::OnResourceLoaded(TResource* LoadedResource, TComponent* TargetComponent)
{
    if (LoadedResource)
    {
        // 如果加载成功，调用具体的资源应用逻辑
        OnResourceLoaded_Impl(LoadedResource, TargetComponent);
    }
    else
    {
        // 如果加载失败，调用空指针处理逻辑
        OnResourceLoaded_Impl(nullptr, TargetComponent);
    }
}

// 实现具体的资源应用逻辑（针对 UStaticMesh 和 UStaticMeshComponent 的特化）
inline void OnResourceLoaded_Impl(UStaticMesh* LoadedResource, UStaticMeshComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        // 如果资源和组件都有效，将资源设置到组件中
        TargetComponent->SetStaticMesh(LoadedResource);

        // 打印日志，表示资源成功加载并应用到组件
        UE_LOG(LogTemp, Log, TEXT("Successfully applied resource to component: %s"), *LoadedResource->GetName());
    }
    else
    {
        // 如果任一项无效，打印警告日志
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply resource to component: Invalid input!"));
    }
}

// 针对 USkeletalMeshComponent 的实现
inline void OnResourceLoaded_Impl(USkeletalMesh* LoadedResource, USkeletalMeshComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        TargetComponent->SetSkeletalMesh(LoadedResource);
        UE_LOG(LogTemp, Log, TEXT("Successfully applied skeletal mesh resource: %s"), *LoadedResource->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply skeletal mesh resource!"));
    }
}

// 针对 UAudioComponent 的实现
inline void OnResourceLoaded_Impl(USoundBase* LoadedResource, UAudioComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        TargetComponent->SetSound(LoadedResource);
        UE_LOG(LogTemp, Log, TEXT("Successfully applied audio resource: %s"), *LoadedResource->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply audio resource!"));
    }
}

// 针对 UAnimSequence 的实现
inline void OnResourceLoaded_Impl(UAnimSequence* LoadedResource, USkeletalMeshComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        TargetComponent->PlayAnimation(LoadedResource, false); // 播放动画，不循环
        UE_LOG(LogTemp, Log, TEXT("Successfully applied animation resource: %s to SkeletalMeshComponent."), *LoadedResource->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply animation resource to SkeletalMeshComponent!"));
    }
}

// 动态加载并应用动画蓝图到 SkeletalMeshComponent
inline void OnResourceLoaded_Impl(UAnimBlueprint* LoadedResource, USkeletalMeshComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        // 设置动画蓝图
        TargetComponent->SetAnimInstanceClass(LoadedResource->GeneratedClass);
        UE_LOG(LogTemp, Log, TEXT("Successfully applied AnimBlueprint resource: %s to SkeletalMeshComponent."), *LoadedResource->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply AnimBlueprint resource to SkeletalMeshComponent!"));
    }
}

// 针对 UParticleSystemComponent 的实现
inline void OnResourceLoaded_Impl(UParticleSystem* LoadedResource, UParticleSystemComponent* TargetComponent)
{
    if (LoadedResource && TargetComponent)
    {
        TargetComponent->SetTemplate(LoadedResource);
        UE_LOG(LogTemp, Log, TEXT("Successfully applied particle system resource: %s"), *LoadedResource->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to apply particle system resource!"));
    }
}

// 实现具体的资源应用逻辑（针对空指针处理的特化）
inline void OnResourceLoaded_Impl(std::nullptr_t, UStaticMeshComponent* TargetComponent)
{
    if (TargetComponent)
    {
        // 如果目标组件有效，将空资源应用到组件（如清除当前的资源）
        UE_LOG(LogTemp, Warning, TEXT("Failed to load resource: Applying nullptr to component!"));
        TargetComponent->SetStaticMesh(nullptr);
    }
    else
    {
        // 如果目标组件无效，打印警告日志
        UE_LOG(LogTemp, Warning, TEXT("Failed to load resource: nullptr target component!"));
    }
}
