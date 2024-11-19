// LINK

//Default
#include "PWGameplayStatics.h"

//Engine
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"

APWPlayerCharacter* UPWGameplayStatics::GetLocalPlayerCharacter(const UObject* WorldContextObj)
{
	APWPlayerController* PWPlayerController = GetLocalPlayerController(WorldContextObj);
	if (IsValid(PWPlayerController) == true)
	{
		 return Cast<APWPlayerCharacter>(PWPlayerController->GetCharacter());
	}

	return nullptr;
}

APWPlayerController* UPWGameplayStatics::GetLocalPlayerController(const UObject* WorldContextObj)
{
	UWorld* World = WorldContextObj->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true && PWPlayerController ->IsLocalController())
			{
				return PWPlayerController;
			}
		}
	}

	return nullptr;
}

APWPlayerController* UPWGameplayStatics::GetOtherPlayerController(const UObject* WorldContextObj)
{
	UWorld* World = WorldContextObj->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true && PWPlayerController ->IsLocalController() == false)
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
