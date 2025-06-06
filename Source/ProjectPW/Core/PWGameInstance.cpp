﻿#include "PWGameInstance.h"
// LINK

//Default
#include "PWGameInstance.h"

//Engine

//Game
#include "Core/Subsystem/PWBGMManageSubsystem.h"
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "Core/Subsystem/PWTurnManageSubsystem.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/DataAsset/PWSoundData.h"
#include "PWAssetLoadManager.h"
#include "PWEventManager.h"

void UPWGameInstance::Init()
{
	Super::Init();

	PWEventManager = NewObject<UPWEventManager>(this, UPWEventManager::StaticClass());
	PWEventManager->AddToRoot();

	PWAssetLoadManager = NewObject<UPWAssetLoadManager>(this, UPWAssetLoadManager::StaticClass());
	PWAssetLoadManager->AddToRoot();

	if (ensure(IsValid(PWGameData) == true))
	{
		PWGameData->Initialize();
	}

	if (ensure(IsValid(PWSoundData) == true))
	{
		PWSoundData->Initialize();
	}
}

void UPWGameInstance::Shutdown()
{
	Super::Shutdown();

	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->RemoveFromRoot();
	}

	if (IsValid(PWAssetLoadManager) == true)
	{
		PWAssetLoadManager->RemoveFromRoot();
	}
}

UPWGameInstance* UPWGameInstance::Get(const UObject* WorldContextObj)
{
	if (IsValid(WorldContextObj))
	{
		if (const UWorld* World = WorldContextObj->GetWorld())
		{
			return World->GetGameInstance<UPWGameInstance>();
		}
	}

	return nullptr;
}

UPWGameData* UPWGameInstance::GetGameData(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetGameData();
}

UPWGameData* UPWGameInstance::GetGameData() const
{
	return 	PWGameData;
}

UPWGameSetting* UPWGameInstance::GetGameSetting(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetGameSetting();
}

UPWGameSetting* UPWGameInstance::GetGameSetting() const
{
	return PWGameSetting;
}

UPWSoundData* UPWGameInstance::GetSoundData(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetSoundData();
}

UPWSoundData* UPWGameInstance::GetSoundData() const
{
	return PWSoundData;
}

UPWEventManager* UPWGameInstance::GetEventManager(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetEventManager();
}

UPWAssetLoadManager* UPWGameInstance::GetAssetLoadManager(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetAssetLoadManager();
}

UPWTurnManageSubsystem* UPWGameInstance::GetPWTurnManageSubsystem() const
{
	return GetSubsystem<UPWTurnManageSubsystem>();
}

UPWSteamMatchMakingSubsystem* UPWGameInstance::GetPWSteamMatchMakingSubsystem() const
{
	return GetSubsystem<UPWSteamMatchMakingSubsystem>();
}

UPWBGMManageSubsystem* UPWGameInstance::GetPWBGMManageSubsystem() const
{
	return GetSubsystem<UPWBGMManageSubsystem>();
}
