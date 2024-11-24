// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerState.h"

//Default
#include "PWPlayerState.generated.h"

enum class ETeamSide : uint8;

USTRUCT()
struct FCharacterAliveData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Transient)
	class APWPlayerCharacter* PlayerCharacter;

	UPROPERTY(Transient)
	bool bIsAlive = true;
};

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
	
	void OnCharacterMoved(ETeamSide InTeamSide, float Distance);

	const FCharacterAliveData GetTeamCharacterData(int32 CharacterNum) const;
	TArray<class APWPlayerCharacter*> GetAliveTeamCharacterList() const;
	TArray<class APWPlayerCharacter*> GetTeamCharacterList() const;
	int32 GetAliveTeamCharacterNum() const;

	void SetIsMyTurn(bool bInIsMyTurn);
	bool IsMyTurn() const { return bIsMyTurn; }
	ETeamSide GetTeamSide() const { return TeamSide; }

	bool IsTeamCharacterInitialized() const { return bIsTeamCharacterInitialized; }

private:

	void SS_LoadCharacters();

	void OnCharacterDead(class APWPlayerCharacter* DeadCharacter);

	UFUNCTION()
	void OnRep_TeamCharacterDataList();

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(Transient, Replicated)
	bool bIsMyTurn;

	UPROPERTY(Transient, Replicated)
	ETeamSide TeamSide;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamCharacterDataList)
	TArray<FCharacterAliveData> TeamCharacterDataList;

	UPROPERTY(transient, Replicated)
	class APawn* CommanderPawn;

	//턴 행동력
	//로컬에서 계산
	UPROPERTY(Transient)
	float CurrentTurnActivePoint = 0.f;

	UPROPERTY(Transient)
	bool bIsTeamCharacterInitialized = false;

	UPROPERTY(Transient)
	class APWPlayerController* OwningPlayerController;

	UPROPERTY(transient)
	bool bIsReadyToMove = false;
};
