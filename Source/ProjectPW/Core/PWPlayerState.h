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

	void SetTeamSide(ETeamSide InTeamSide);
	void LoadCharacters();
	
	void OnCharacterMoved(ETeamSide InTeamSide, float Distance);

	void SetIsMyTurn(bool bInIsMyTurn);
	bool IsMyTurn() const { return bIsMyTurn; }
	ETeamSide GetTeamSide() const { return TeamSide; }

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(Transient, Replicated)
	bool bIsMyTurn;

	UPROPERTY(Transient, Replicated)
	ETeamSide TeamSide;

	UPROPERTY(Transient)
	TArray<class APWPlayerCharacter*> CharacterList;

	//턴 행동력
	//로컬에서 계산
	UPROPERTY(Transient)
	float CurrentTurnActivePoint = 0.f;
};
