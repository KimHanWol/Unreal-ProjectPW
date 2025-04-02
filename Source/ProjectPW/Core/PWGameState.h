// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameStateBase.h"
#include "Data/PWGameEnum.h"

//Default
#include "PWGameState.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Transient)
	class APWPlayerCharacter* PlayerCharacter;

	UPROPERTY(Transient)
	ETeamSide PlayerTeamSide;
};

UCLASS()
class PROJECTPW_API APWGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void OnStartGame(int32 InMaxPlayerCount);
	void NextTurn();

	int32 GetCurrentPlayerTurn() const { return CurrentPlayersTurn; }

	void OnCharacterAliveStateChanged(class APWPlayerCharacter* TargetCharacter, bool bIsAlive);
	void OnPlayerGameOver(class APWPlayerController* PlayerController, bool bLose);
	void OnEntireGameOver();

	FName GetSelectedLevelKey() const;
	void SetSelectedLevelKey(FName InSelectedLevelKey);

public:

	DECLARE_MULTICAST_DELEGATE(FGameStateCharacterAliveStateChanged)
	FGameStateCharacterAliveStateChanged GameStateCharacterAliveStateChangedDelegate;

private:

	// 선택된 레벨
	UPROPERTY(Transient)
	FName SelectedLevelKey;

	// 플레이어 차례
	UPROPERTY(Transient)
	int32 CurrentPlayersTurn = 0;

	// 게임 라운드 0 ~ (0, 1 차례가 지나고 매 턴 마다 1씩 쌓임)
	UPROPERTY(Transient)
	int32 CurrentRoundIndex = 0;

	//플레이어 수
	UPROPERTY(Transient)
	int32 MaxPlayerCount = 0;

	//Controller, IsGameOver
	UPROPERTY(Transient)
	TMap<class APWPlayerController*, bool> PlayerControllerGameStateMap;

	//Character, IsAlive
	UPROPERTY(Transient)
	TMap<class APWPlayerCharacter*, bool> PlayerCharacterAliveDataMap;
};
