// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameModeBase.h"

//Default
#include "PWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API APWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player);

private:
	
	AActor* GetSpawnPoints(int32 PlayerIndex) const;

};
