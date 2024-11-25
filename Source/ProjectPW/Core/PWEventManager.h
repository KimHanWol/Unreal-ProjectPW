// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/GameInstance.h"

//Default
#include "PWEventManager.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API UPWEventManager : public UObject
{
	GENERATED_BODY()

public:

	static UPWEventManager* Get(const UObject* WorldContextObject);

public:

	//Health
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FHealthChanged, AActor* TargetActor, float MaxHealth, float CurrentHealth);
	FHealthChanged HealthChangedDelegate;

	DECLARE_MULTICAST_DELEGATE(FNextTurn);
	FNextTurn NextTurnDelegate;

	//Game
	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterMoved, float CurrentTurnActivePoint);
	FCharacterMoved TeamCharacterMovedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSelected, int32 CharacterNum);
	FCharacterSelected CharacterSelectedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPossessed, APawn* PossessedPawn, bool bIsCommander);
	FPlayerPossessed PlayerPossessedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterAliveStateChanged, class APWPlayerCharacter* TargetCharacter, bool bIsAlive);
	FCharacterAliveStateChanged CharacterAliveStateChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetIsOnCrosshair, class APWPlayerCharacter* AimingCharacter, bool bIsOnCrosshair);
	FTargetIsOnCrosshair TargetIsOnCrosshairDelegate;

	DECLARE_MULTICAST_DELEGATE(FTurnChanged);
	FTurnChanged TurnChangedDelegate;

	//System
	DECLARE_MULTICAST_DELEGATE_TwoParams(FGameOver, class APWPlayerController* PlayerController, bool bWon);
	FGameOver GameOverDelegate;
};
