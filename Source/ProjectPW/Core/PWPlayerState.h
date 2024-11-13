// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerState.h"

//Default
#include "PWPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API APWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	void SetIsMyTurn(bool bInIsMyTurn);
	bool IsMyTurn() const { return bIsMyTurn; }

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(Transient)
	bool bIsMyTurn;
};
