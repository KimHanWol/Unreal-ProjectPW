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
}
