// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerStart.h"

//Default
#include "PWPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API APWPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:

	int32 GetPlayerIndex() const { return PlayerIndex; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayerIndex = 0;
};
