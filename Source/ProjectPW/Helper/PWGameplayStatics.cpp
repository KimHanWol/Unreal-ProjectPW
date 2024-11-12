// LINK

//Default
#include "PWGameplayStatics.h"

//Engine
#include "GameFramework/PlayerState.h"

//Game

APlayerController* UPWGameplayStatics::GetLocalPlayerController(const UObject* WorldContextObj)
{
	UWorld* World = WorldContextObj->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && PlayerController->IsLocalController() && IsValid(PlayerController->PlayerState))
			{
				return PlayerController;
			}
		}
	}

	return nullptr;
}