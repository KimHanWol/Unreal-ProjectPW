﻿// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Core/InputHandler/InputHandlerBase.h"

//Default
#include "CharacterInputHandler.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UCharacterInputHandler : public UInputHandlerBase
{
	GENERATED_BODY()

public:

	virtual void SetupKeyBindings(class APWPlayerController* InPWPlayerController) override;

private:

	void Move(const struct FInputActionValue& Value);
	void Jump(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void LoadPrevState(const struct FInputActionValue& Value);
	void Execute_Main_Triggered(const struct FInputActionValue& Value);
	void Execute_Main_Completed(const struct FInputActionValue& Value);
	void Execute_Sub_Triggered(const struct FInputActionValue& Value);
	void Execute_Sub_Completed(const struct FInputActionValue& Value);
	void Select_ESC(const struct FInputActionValue& Value);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* LoadAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* ExecuteAction_Main;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* ExecuteAction_Sub;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_ESC;

	UPROPERTY(Transient)
	class APWPlayerController* PWPlayerController;
};
