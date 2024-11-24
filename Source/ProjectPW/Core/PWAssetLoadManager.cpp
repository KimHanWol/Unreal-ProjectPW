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
