// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerState.h"

//Default
#include "PWPlayerState.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void SS_InitializePlayerState(ETeamSide InTeamSide);
	void SetCommanderPawn(APawn* InCommanderPawn);
	APawn* GetCommanderPawn();
	
	void OnCharacterMoved(float Distance);

	class APWPlayerCharacter* GetTeamCharacter(int32 CharacterNum) const;
	TArray<class APWPlayerCharacter*> GetAliveTeamCharacterList() const;
	TArray<class APWPlayerCharacter*> GetTeamCharacterList() const;
	int32 GetAliveTeamCharacterNum() const;

	void SetIsMyTurn(bool bInIsMyTurn);
	bool IsMyTurn() const { return bIsMyTurn; }
	ETeamSide GetTeamSide() const { return TeamSide; }

	bool IsTeamCharacterInitialized() const { return bIsTeamCharacterInitialized; }

	float GetCurrentTurnActivePoint() const { return CurrentTurnActivePoint; }
	void SetCurrentTurnActivePoint(float InCurrentTurnActivePoint);

	void SS_LoadCharacters();

	UFUNCTION()
	void OnRep_TeamCharacterList();

	void GameOver() { bGameOver = true; }
	bool IsGameOver() const { return bGameOver; }

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(Transient, Replicated)
	bool bIsMyTurn;

	UPROPERTY(Transient, Replicated)
	ETeamSide TeamSide;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamCharacterList)
	TArray<class APWPlayerCharacter*> TeamCharacterList;

	UPROPERTY(transient, Replicated)
	class APawn* CommanderPawn;

	//턴 행동력
	//로컬에서 계산
	//비용 생각해서 Replicate 하지 않고 필요할 때 마다 RPC로 확인
	UPROPERTY(Transient)
	float CurrentTurnActivePoint = 0.f;

	UPROPERTY(Transient)
	bool bIsTeamCharacterInitialized = false;

	UPROPERTY(Transient)
	class APWPlayerController* OwningPlayerController;

	UPROPERTY(transient)
	bool bIsReadyToMove = false;

	//Server only
	UPROPERTY(Transient)
	bool bGameOver;
};
