// LINK

//Default
#include "PWGameInstance.h"

//Engine

//Game
#include "Data/DataAsset/PWGameData.h"
#include "Helper/PWGameplayStatics.h"

void UPWGameInstance::Init()
{
	Super::Init();

	TArray<FSoftObjectPath> ItemsToStream;
	if (PWGameDataPtr.IsNull() == false)
	{
		ItemsToStream.AddUnique(PWGameDataPtr.ToSoftObjectPath());
	}
	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);
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