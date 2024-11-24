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
	int32 GetCurrentRoundIndex() const { return CurrentRoundIndex; } // TODO: MainWidget 에 데이터 추가하기

private:

	// 0 or 1
	UPROPERTY(Transient)
	int32 CurrentPlayersTurn = 0;

	// 0 ~ (0, 1 차례가 지나고 매 턴 마다 1씩 쌓임)
	UPROPERTY(Transient)
	int32 CurrentRoundIndex = 0;

	//플레이어 수
	UPROPERTY(Transient)
	int32 MaxPlayerCount = 0;
};
