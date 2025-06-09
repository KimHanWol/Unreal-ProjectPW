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
enum class ECharacterType : uint8;

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

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTeamCharacterAllSpawned, const class APWPlayerController* TargetPlayerController, const TArray<TWeakObjectPtr<class APWPlayerCharacter>>& TargetCharacterList);
	FTeamCharacterAllSpawned TeamCharacterAllSpawnedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterTriedToUseSkill, float TurnActivePointCost, bool bIsSuccessed);
	FCharacterTriedToUseSkill CharacterTriedToUseSkillDelegate;

	//Player
	DECLARE_MULTICAST_DELEGATE(FClientTeamSideInitialized);
	FClientTeamSideInitialized ClientTeamSideInitializedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSelectedForSpawn, ECharacterType SelectedCharacterType);
	FCharacterSelectedForSpawn CharacterSelectedForSpawnDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSelected, int32 CharacterNum);
	FCharacterSelected CharacterSelectedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPossessed, APawn* PossessedPawn, bool bIsCommander);
	FPlayerPossessed PlayerPossessedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetIsOnCrosshair, class APWPlayerCharacter* AimingCharacter, bool bIsOnCrosshair);
	FTargetIsOnCrosshair TargetIsOnCrosshairDelegate;

	//Turn

	DECLARE_MULTICAST_DELEGATE(FBattleLevelSettingFinished);
	FBattleLevelSettingFinished BattleLevelSettingFinished; // Team Side 및 Commander 스폰 까지 다 된 상태

	DECLARE_MULTICAST_DELEGATE(FRequestNextTurn);
	FRequestNextTurn RequestNextTurnDelegate;

	DECLARE_MULTICAST_DELEGATE(FTurnChanged);
	FTurnChanged TurnChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FGameOver, class APWPlayerController* PlayerController, bool bWon);
	FGameOver GameOverDelegate;

	//UI
	DECLARE_MULTICAST_DELEGATE_OneParam(FShowWidget, EWidgetType TargetWidgetType);
	FShowWidget ShowWidgetDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FShowNotiWidget, const FText& NotificationText);
	FShowNotiWidget ShowNotiWidgetDelegate;

	//MatchMaking
	DECLARE_MULTICAST_DELEGATE(FMatchMakingSuccess);
	FMatchMakingSuccess MatchMakingSuccessDelegate;
};
