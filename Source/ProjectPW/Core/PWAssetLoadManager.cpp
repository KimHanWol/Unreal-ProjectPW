// LINK

//Default
#include "PWAssetLoadManager.h"

//Engine
#include "Engine/StreamableManager.h"

//Game
#include "Engine/AssetManager.h"
#include "PWGameInstance.h"

UPWAssetLoadManager* UPWAssetLoadManager::Get(const UObject* WorldContextObject)
{
	return UPWGameInstance::GetAssetLoadManager(WorldContextObject);
}

void UPWAssetLoadManager::Initialize()
{
	UPWGameInstance* PWGameInstance = UPWGameInstance::Get(this);
	if (ensure(IsValid(PWGameInstance) == true))
	{
	}
}
