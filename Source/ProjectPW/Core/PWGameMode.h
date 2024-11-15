// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameModeBase.h"

//Default
#include "PWGameMode.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player);

private:

	ETeamSide ChooseTeamSide(AController* Player);
	AActor* GetSpawnPoints(ETeamSide TeamSide) const;
	
	void StartGame();
	void TextTurn();

};
