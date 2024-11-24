// LINK

//Default
#include "PWTurnManageSubsystem.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWGameInstance.h"
#include "Core/PWGameState.h"
#include "Helper/PWGameplayStatics.h"

UPWTurnManageSubsystem* UPWTurnManageSubsystem::Get(const UObject* WorldContextObj)
{
    UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
    if (IsValid(PWGameInst) == true)
    {
        return PWGameInst->GetPWTurnManageSubsystem();
    }

    return nullptr;
}

void UPWTurnManageSubsystem::StartGame(const TArray<APWPlayerController*>& InGamePlayerControllerList)
{
    GamePlayerControllerList = InGamePlayerControllerList;

    for (int32 i = 0; i < GamePlayerControllerList.Num(); i++)
	{
		APWPlayerController* PlayerController = GamePlayerControllerList[i];
		if (IsValid(PlayerController) == true)
		{
			bool bMyTurn = i == CurrentTurnIndex; //0번 부터 시작
			PlayerController->SC_ChangeTurn(bMyTurn);
		}
	}

    PWGameState = Cast<APWGameState>(UPWGameplayStatics::GetGameState(this));
    if (IsValid(PWGameState) == true)
    {
        PWGameState->OnStartGame(GamePlayerControllerList.Num());
    }

    BindEvents();
}

void UPWTurnManageSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UnbindEvents();
}

void UPWTurnManageSubsystem::BindEvents()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->NextTurnDelegate.AddUObject(this, &UPWTurnManageSubsystem::NextTurn);
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UPWTurnManageSubsystem::OnPossessed);
	}
}

void UPWTurnManageSubsystem::UnbindEvents()
{
    UPWEventManager* PWEventManager = UPWEventManager::Get(this);
    if (IsValid(PWEventManager) == true)
    {
        PWEventManager->NextTurnDelegate.RemoveAll(this);
        PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
    }
}

void UPWTurnManageSubsystem::NextTurn()
{
    if (IsValid(PWGameState) == true)
    {
        PWGameState->NextTurn();
        CurrentTurnIndex = PWGameState->GetCurrentPlayerTurn();
    }

    for (int32 i = 0; i < GamePlayerControllerList.Num(); i++)
	{
		APWPlayerController* PlayerController = GamePlayerControllerList[i];
		if (IsValid(PlayerController) == true)
		{
			bool bMyTurn = i == CurrentTurnIndex; //0번 부터 시작
			PlayerController->SC_ChangeTurn(bMyTurn);
		}
	}
}

void UPWTurnManageSubsystem::OnPossessed(APawn* PossessedPawn, bool bIsCommander)
{
    
}
