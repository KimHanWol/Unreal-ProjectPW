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

	//Game
	DECLARE_MULTICAST_DELEGATE_OneParam(FInitialPossessed, class APWPlayerController* PWPlayerController);
	FInitialPossessed InitialPossessDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterMoved, float CurrentTurnActivePoint);
	FCharacterMoved TeamCharacterMovedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSelected, bool bIsCommander);
	FCharacterSelected CharacterSelectedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDead, class APWPlayerCharacter* DeadCharacter);
	FCharacterDead CharacterDeadDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FTargetIsOnCrosshair, bool bIsOnCrosshair);
	FTargetIsOnCrosshair TargetIsOnCrosshairDelegate;

	//System
	DECLARE_MULTICAST_DELEGATE_OneParam(FGameOver, bool bWon);
	FGameOver GameOverDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTeamCharcterLoaded, ETeamSide TeamSide, const TArray<APWPlayerCharacter*>& TeamCharacterList);
	FTeamCharcterLoaded TeamCharcterLoadedDelegate;

};
