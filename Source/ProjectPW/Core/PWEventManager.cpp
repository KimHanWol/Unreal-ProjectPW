// LINK

//Default
#include "PWEventManager.h"

//Engine

//Game
#include "PWGameInstance.h"

UPWEventManager* UPWEventManager::Get(const UObject* WorldContextObject)
{
	return UPWGameInstance::GetEventManager(WorldContextObject);
}