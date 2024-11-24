// LINK

//Default
#include "PWGameState.h"

//Engine

//Game

void APWGameState::OnStartGame(int32 InMaxPlayerCount)
{
	CurrentPlayersTurn = 0;
	CurrentRoundIndex = 0;
	MaxPlayerCount = InMaxPlayerCount;
}

void APWGameState::NextTurn()
{
	++CurrentPlayersTurn;

	if (CurrentPlayersTurn >= MaxPlayerCount)
	{
		CurrentPlayersTurn = 0;
		++CurrentRoundIndex;
	}

	UE_LOG(LogTemp, Log, TEXT("Turn Changed : %d Turn, Player %d"), CurrentRoundIndex + 1, CurrentPlayersTurn);
}
