// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/GameInstance.h"

//Default
#include "PWEventManager.generated.h"

/**
 * 
 */
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

	//System
	DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDead, class APWPlayerCharacter* DeadCharacter);
	FCharacterDead CharacterDeadDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FGameOver, bool bWon);
	FGameOver GameOverDelegate;

};
