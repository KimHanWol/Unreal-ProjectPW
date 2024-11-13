// LINK

//Default
#include "PWPlayerController.h"

//Engine

//Game
#include "Core/PWPlayerState.h"
#include "UI/MasterWidget.h"

void APWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MasterWidget))
	{
		MasterWidget->AddToViewport();
	}
}

void APWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetIsMyTurn(IsMyTurn);
	}

	PlayerStateLoadedDelegate.Broadcast();
}

void APWPlayerController::SC_ChangeTurn_Implementation(bool bMyTurn)
{
	IsMyTurn = bMyTurn;

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetIsMyTurn(IsMyTurn);
	}

	TurnChangedDelegate.Broadcast(bMyTurn);
}
