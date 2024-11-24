// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Subsystems/GameInstanceSubsystem.h"

//Default
#include "PWTurnManageSubsystem.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API UPWTurnManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	static UPWTurnManageSubsystem* Get(const UObject* WorldContextObj);

	void StartGame(const TArray<class APWPlayerController*>& InGamePlayerControllerList);

protected:
	
	virtual void Deinitialize() override;

private:

	void BindEvents();
	void UnbindEvents();

	void NextTurn();
	void OnPossessed(APawn* PossessedPawn, bool bIsCommander);

private:

	UPROPERTY(Transient)
	int32 CurrentTurnIndex = 0;

	UPROPERTY(Transient)
	class APWGameState* PWGameState;

	UPROPERTY(Transient)
	TArray<APWPlayerController*> GamePlayerControllerList;
};

