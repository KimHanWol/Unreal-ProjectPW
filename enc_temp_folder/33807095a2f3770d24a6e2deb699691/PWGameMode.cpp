// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "Kismet/GameplayStatics.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWPlayerStart.h"
#include "PWGameState.h"

#define GET_PLAYER_INTURN(InWorldContext, InCurrentPlayersTurn) \
	(Cast<APWPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext->GetWorld(), InCurrentPlayersTurn)))

#define GET_PLAYER_NOTTURN(InWorldContext, InCurrentPlayersTurn) \
	(Cast<APWPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext->GetWorld(), 1 - InCurrentPlayersTurn)))


void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (GetNumPlayers() == 2)
	{
		StartGame();
	}
}

AActor* APWGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 현재 플레이어의 수에 따라서 플레이어의 Index 설정 
	int32 PlayerNum = GetNumPlayers();
	return GetSpawnPoints(PlayerNum - 1);
}

void APWGameMode::StartGame()
{
	APWPlayerController* PlayerControllerInTurn = GET_PLAYER_INTURN(this, 0);
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->SC_ChangeTurn(true);
	}

	APWPlayerController* PlayerControllerNotTurn = GET_PLAYER_NOTTURN(this, 0);
	if (IsValid(PlayerControllerNotTurn) == true)
	{
		PlayerControllerNotTurn->SC_ChangeTurn(false);
	}

	UE_LOG(LogTemp, Log, TEXT("Turn Started : 0 Turn, Player 0"));
}

void APWGameMode::TextTurn()
{
	APWGameState* PWGameState = Cast<APWGameState>(UGameplayStatics::GetGameState(this));
	if (IsValid(PWGameState) == false)
	{
		ensure(false);
		return;
	}

	int32 CurrentPlayersTurn = PWGameState->GetCurrentPlayerTurn();

	APWPlayerController* PlayerControllerInTurn = GET_PLAYER_INTURN(this, CurrentPlayersTurn);
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->SC_ChangeTurn(false);
	}

	APWPlayerController* PlayerControllerNotTurn = GET_PLAYER_NOTTURN(this, CurrentPlayersTurn);
	if (IsValid(PlayerControllerNotTurn) == true)
	{
		PlayerControllerNotTurn->SC_ChangeTurn(true);
	}

	PWGameState->NextTurn();

	UE_LOG(LogTemp, Log, TEXT("Turn Changed : %d Turn, Player %d"), PWGameState->GetCurrentRoundIndex() + 1, PWGameState->GetCurrentPlayerTurn());
}

AActor* APWGameMode::GetSpawnPoints(int32 PlayerIndex) const
{
	TArray<AActor*> PlayerStartList;
	UGameplayStatics::GetAllActorsOfClass(this, APWPlayerStart::StaticClass(), PlayerStartList);

	for (AActor* PlayerStartActor : PlayerStartList)
	{
		const APWPlayerStart* PlayerStart = Cast<APWPlayerStart>(PlayerStartActor);
		if (IsValid(PlayerStart) == true)
		{
			if (PlayerIndex == PlayerStart->GetPlayerIndex()) 
			{
				return PlayerStartActor;
			}
		}
	}

	return nullptr;
}