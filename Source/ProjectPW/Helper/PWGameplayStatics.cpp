// LINK

//Default
#include "PWGameplayStatics.h"

//Engine
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Particles/ParticleSystem.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"

APWPlayerController* UPWGameplayStatics::GetLocalPlayerController(const UObject* WorldContextObj)
{
	UWorld* World = WorldContextObj->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (PWPlayerController  && PWPlayerController ->IsLocalController())
			{
				return PWPlayerController;
			}
		}
	}

	return nullptr;
}

APWPlayerState* UPWGameplayStatics::GetLocalPlayerState(const UObject* WorldContextObj)
{
	APWPlayerState* PWPlayerState = nullptr;

	APWPlayerController* PWPlayerController = GetLocalPlayerController(WorldContextObj);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true)
		{
			return PWPlayerState;
		}
	}

	return nullptr;
}

void UPWGameplayStatics::AsyncLoadAsset(const FSoftObjectPath& AsyncLoadAssetPath)
{
	TArray<FSoftObjectPath> AsnycLoadAssetPathList;
	AsnycLoadAssetPathList.Add(AsyncLoadAssetPath);
	AsyncLoadAsset(AsnycLoadAssetPathList);
}

void UPWGameplayStatics::AsyncLoadAsset(const TArray<FSoftObjectPath>& AsyncLoadAssetPathList)
{
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	TArray<FSoftObjectPath> ItemsToStream;
	ItemsToStream.Append(AsyncLoadAssetPathList);
    Streamable.RequestAsyncLoad(ItemsToStream);
}
