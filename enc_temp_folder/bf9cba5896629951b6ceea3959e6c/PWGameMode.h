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

	virtual void BeginPlay() override;

private:

	void LoadCharacter();

	void BindEvents();
	void OnCharcterDead(class APWPlayerCharacter* DeadCharacter);
	void CheckGameOver();
	void OnGameOver();

	ETeamSide ChooseTeamSide(AController* Player);
	AActor* GetSpawnPoints(ETeamSide TeamSide) const;
	
	void StartGame();
	void TextTurn();

private:

	UPROPERTY(Transient)
	TMap<class APWPlayerCharacter*, ETeamSide> AliveCharacterMap;

};
