// LINK

//Default
#include "PWGameInstance.h"

//Engine

//Game
#include "Data/DataAsset/PWGameData.h"
#include "Helper/PWGameplayStatics.h"
#include "PWEventManager.h"

void UPWGameInstance::Init()
{
	Super::Init();

	TArray<FSoftObjectPath> ItemsToStream;
	if (PWGameDataPtr.IsNull() == false)
	{
		ItemsToStream.AddUnique(PWGameDataPtr.ToSoftObjectPath());
	}
	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);

	PWEventManager = NewObject<UPWEventManager>(this, UPWEventManager::StaticClass());
	PWEventManager->AddToRoot();
}

void UPWGameInstance::Shutdown()
{
	Super::Shutdown();

	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->RemoveFromRoot();
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

UPWGameData* UPWGameInstance::GetGameData()
{
	if (PWGameDataPtr.IsNull() == false)
	{
		return PWGameDataPtr.LoadSynchronous();
	}

	return nullptr;
}

UPWEventManager* UPWGameInstance::GetEventManager(const UObject* WorldContextObj)
{
	return UPWGameInstance::Get(WorldContextObj)->GetEventManager();
}
