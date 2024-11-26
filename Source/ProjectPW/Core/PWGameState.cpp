// LINK

//Default
#include "PWGameState.h"

//Engine

//Game
#include "Helper/PWGameplayStatics.h"
#include "PWEventManager.h"

void APWGameState::OnStartGame(int32 InMaxPlayerCount)
{
	CurrentPlayersTurn = 0;
	CurrentRoundIndex = 0;
	MaxPlayerCount = InMaxPlayerCount;

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.AddUObject(this, &APWGameState::OnPlayerGameOver);
		PWEventManager->CharacterAliveStateChangedDelegate.AddUObject(this, &APWGameState::OnCharacterAliveStateChanged);
	}

	const TArray<APWPlayerController*>& PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	for (APWPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerControllerGameStateMap.Add(TTuple<APWPlayerController*, bool>(PlayerController, false));
	}

	const TArray<APWPlayerCharacter*>& PlayerCharacterList = UPWGameplayStatics::GetAllPlayerCharacter(this);
	for (APWPlayerCharacter* PlayerCharacter : PlayerCharacterList)
	{
		PlayerCharacterAliveDataMap.Add(TTuple<APWPlayerCharacter*, bool>(PlayerCharacter, true));
	}
}

void APWGameState::NextTurn()
{
	++CurrentPlayersTurn;

	if (CurrentPlayersTurn >= MaxPlayerCount)
	{
		CurrentPlayersTurn = 0;
		++CurrentRoundIndex;
	}

	TArray<bool> PlayerGameOverList;
	PlayerControllerGameStateMap.GenerateValueArray(PlayerGameOverList);
	if (PlayerGameOverList[CurrentPlayersTurn] == true)
	{
		NextTurn();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Turn Changed : %d Turn, Player %d"), CurrentRoundIndex + 1, CurrentPlayersTurn);
}

void APWGameState::OnPlayerGameOver(APWPlayerController* PlayerController, bool bLose)
{
	if (PlayerControllerGameStateMap.Contains(PlayerController) == true)
	{
		PlayerControllerGameStateMap[PlayerController] = bLose;
	}
}

void APWGameState::OnEntireGameOver()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.RemoveAll(this);
		PWEventManager->CharacterAliveStateChangedDelegate.RemoveAll(this);
	}
}

void APWGameState::OnCharacterAliveStateChanged(APWPlayerCharacter* TargetCharacter, bool bIsAlive)
{
	if(PlayerCharacterAliveDataMap.Contains(TargetCharacter) == true)
	{
		PlayerCharacterAliveDataMap[TargetCharacter] = bIsAlive;
	}

	GameStateCharacterAliveStateChangedDelegate.Broadcast();
}
