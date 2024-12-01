// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Components/ActorComponent.h"

//Default
#include "PWPlayerInputComponent.generated.h"


UCLASS()
class PROJECTPW_API UPWPlayerInputComponent: public UActorComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:

	void InitMappingContext();

	void SetSpawnCharacterInputEnabled(bool bEnabled);
	void SetInGameInputEnabled(bool bEnableCommanderInput, bool bEnableCharacterInput);

private:

	void TryCreateInputHandler();
	void SetMouseInputToGameAndUI(bool bInShowWithCursor = true);
	void SetMouseInputToGame();

	void OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputMappingContext* DefaultContext;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCommanderInputHandler> CommanderInputHandlerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterInputHandler> CharacterInputHandlerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USpawnCharacterInputHandler> SpawnCharacterInputHandlerClass;

	UPROPERTY(transient)
	class APWPlayerController* PWPlayerController;

	UPROPERTY(transient)
	class UCommanderInputHandler* CommanderInputHandler;

	UPROPERTY(transient)
	class UCharacterInputHandler* CharacterInputHandler;

	UPROPERTY(transient)
	class USpawnCharacterInputHandler* SpawnCharacterInputHandler;

};