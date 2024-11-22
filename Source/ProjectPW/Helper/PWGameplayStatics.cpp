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
#include "Data/PWGameEnum.h"

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

APWPlayerController* UPWGameplayStatics::GetOtherPlayerController(class APWPlayerController* CurrentPlayerController)
{
	//Server Only
	if (IsValid(CurrentPlayerController) == false || CurrentPlayerController->HasAuthority() == false)
	{
		return nullptr;
	}

	if (IsValid(CurrentPlayerController) == false)
	{
		return nullptr;
	}

	UWorld* World = CurrentPlayerController->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true && PWPlayerController != CurrentPlayerController)
			{
				return PWPlayerController;
			}
		}
	}
	return nullptr;
}

TArray<APWPlayerController*> UPWGameplayStatics::GetAllPlayerController(const UObject* WorldContextObj)
{
	TArray<APWPlayerController*> AllPlayerControllerList;

	UWorld* World = WorldContextObj->GetWorld();
	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true)
			{
				AllPlayerControllerList.Add(PWPlayerController);
			}
		}
	}

	return AllPlayerControllerList;
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

ETeamSide UPWGameplayStatics::GetLocalPlayerTeamSide(const UObject* WorldContextObj)
{
	APWPlayerController* LocalPlayerController = GetLocalPlayerController(WorldContextObj);
	if (IsValid(LocalPlayerController) == true)
	{
		return LocalPlayerController->GetTeamSide();
	}

	ensure(false);
	return ETeamSide::Red;
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
