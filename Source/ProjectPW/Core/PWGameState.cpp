// LINK

//Default
#include "PWGameState.h"

//Engine

//Game

void APWGameState::NextTurn()
{
	++CurrentPlayersTurn;

	if (CurrentPlayersTurn > 1)
	{
		CurrentPlayersTurn = 0;
		++CurrentRoundIndex;
	}

	UE_LOG(LogTemp, Log, TEXT("Turn Changed : %d Turn, Player %d"), CurrentRoundIndex + 1, CurrentPlayersTurn);
}
