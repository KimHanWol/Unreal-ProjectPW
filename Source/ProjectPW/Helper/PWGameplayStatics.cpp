// LINK

//Default
#include "PWGameplayStatics.h"

//Engine
#include "EngineUtils.h"
#include "GameFramework/Character.h"

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

TArray<class APWPlayerCharacter*> UPWGameplayStatics::GetAllPlayerCharacter(const UObject* WorldContextObj)
{
	TArray<APWPlayerCharacter*> PlayerCharacterList;

	UWorld* World = WorldContextObj->GetWorld();
	if (IsValid(World))
	{
		for (TActorIterator<APWPlayerCharacter> ActorItr(World); ActorItr; ++ActorItr)
		{		
			APWPlayerCharacter* PWPlayerController = Cast<APWPlayerCharacter>(*ActorItr);
			if (IsValid(PWPlayerController) == true)
			{
				PlayerCharacterList.Add(PWPlayerController);
			}
		}
	}
	
	return PlayerCharacterList;
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