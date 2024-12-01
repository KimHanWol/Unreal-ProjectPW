// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/GameInstance.h"

//Default
#include "PWEventManager.generated.h"

enum class ETeamSide : uint8;
enum class EWidgetType : uint8;

UCLASS()
class PROJECTPW_API UPWEventManager : public UObject
{
	GENERATED_BODY()

public:

	static UPWEventManager* Get(const UObject* WorldContextObject);

public:

	//Character
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FHealthChanged, AActor* TargetActor, float MaxHealth, float CurrentHealth);
	FHealthChanged HealthChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterAliveStateChanged, class APWPlayerCharacter* TargetCharacter, bool bIsAlive);
	FCharacterAliveStateChanged CharacterAliveStateChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterMoved, float CurrentTurnActivePoint);
	FCharacterMoved TeamCharacterMovedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSpawned, class APWPlayerCharacter* TargetCharacter);
	FCharacterSpawned CharacterSpawnedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FTeamCharacterAllSpawned, const TArray<class APWPlayerCharacter*>& TargetCharacterList);
	FTeamCharacterAllSpawned TeamCharacterAllSpawnedDelegate;

	//Player
	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSelected, int32 CharacterNum);
	FCharacterSelected CharacterSelectedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPossessed, APawn* PossessedPawn, bool bIsCommander);
	FPlayerPossessed PlayerPossessedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetIsOnCrosshair, class APWPlayerCharacter* AimingCharacter, bool bIsOnCrosshair);
	FTargetIsOnCrosshair TargetIsOnCrosshairDelegate;

	//Turn
	DECLARE_MULTICAST_DELEGATE(FRequestNextTurn);
	FRequestNextTurn RequestNextTurnDelegate;

	DECLARE_MULTICAST_DELEGATE(FTurnChanged);
	FTurnChanged TurnChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FGameOver, class APWPlayerController* PlayerController, bool bWon);
	FGameOver GameOverDelegate;

	//UI
	DECLARE_MULTICAST_DELEGATE_OneParam(FShowWidget, EWidgetType TargetWidgetType);
	FShowWidget ShowWidgetDelegate;
};
