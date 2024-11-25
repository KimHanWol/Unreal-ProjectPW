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

	virtual void StartPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

private:

	void ReadyToStart();
	void StartGame();

	void BindEvents();
	void CheckGameOver();

	void OnCharcterAliveStateChanged(class APWPlayerCharacter* TargetCharacter, bool bIsAlive);
	void OnInitialPossess(class APWPlayerController* SelfPlayerController);
	void OnGameOver();

	void ChooseTeamSide(AController* Player);
	AActor* GetCommanderPointActor(ETeamSide TeamSide) const;

	void TransformCommanderPawns();

	int32 GetCurrentPlayerTurn();

private:

	UPROPERTY(Transient)
	class APWPlayerController* RedTeamPlayerController;

	UPROPERTY(Transient)
	class APWPlayerController* BlueTeamPlayerController;

	UPROPERTY(Transient)
	int32 InitialPossessedCount = 0;

};
