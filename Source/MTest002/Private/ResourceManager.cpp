// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h" // 用于屏幕调试信息

// 获取单例实例
ResourceManager& ResourceManager::Get()
{
    static ResourceManager Instance;
    return Instance;
}

// 构造函数
ResourceManager::ResourceManager()
{
    // 可选初始化逻辑
}

// 析构函数
ResourceManager::~ResourceManager()
{
    ClearCache();
}
// 清空缓存
void ResourceManager::ClearCache()
{
    ResourceCache.Empty();
}

