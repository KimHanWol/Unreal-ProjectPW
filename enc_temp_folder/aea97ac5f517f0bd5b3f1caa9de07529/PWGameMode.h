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

private:

	void ReadyToStart();
	void StartGame();
	void CheckGameOver();
	void OnGameOver();
	void TransformCommanderPawns();

	void OnInitialPossess(class APWPlayerController* SelfPlayerController);

	AActor* GetCommanderPointActor(ETeamSide TeamSide) const;

private:

	//처음 폰에 빙의된 클라이언트 수
	UPROPERTY(Transient)
	int32 InitialPossessedCount = 0;

	UPROPERTY(Transient)
	int32 TotalPlayerCount;
};
