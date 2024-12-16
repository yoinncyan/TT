// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h" // ������Ļ������Ϣ

// ��ȡ����ʵ��
ResourceManager& ResourceManager::Get()
{
    static ResourceManager Instance;
    return Instance;
}

// ���캯��
ResourceManager::ResourceManager()
{
    // ��ѡ��ʼ���߼�
}

// ��������
ResourceManager::~ResourceManager()
{
    ClearCache();
}
// ��ջ���
void ResourceManager::ClearCache()
{
    ResourceCache.Empty();
}

