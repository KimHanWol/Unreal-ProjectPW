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

	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player);

private:
	
	void StartGame();
	void TextTurn();

	AActor* GetSpawnPoints(int32 PlayerIndex) const;
};
