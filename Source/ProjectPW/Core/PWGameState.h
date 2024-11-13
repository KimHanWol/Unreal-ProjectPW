// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameStateBase.h"

//Default
#include "PWGameState.generated.h"

UENUM(BlueprintType)
enum class ETeamSide : uint8
{
	Red,
	Blue,
};

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

	void NextTurn();

	int32 GetCurrentPlayerTurn() const { return CurrentPlayersTurn; }
	int32 GetCurrentRoundIndex() const { return CurrentRoundIndex; }

private:

	// 0 or 1
	UPROPERTY(Transient)
	int32 CurrentPlayersTurn = 0;

	// 0 ~ (0, 1 차례가 지나고 매 턴 마다 1씩 쌓임)
	UPROPERTY(Transient)
	int32 CurrentRoundIndex = 0;
};
