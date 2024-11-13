// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Core/InputHandler/InputHandlerBase.h"

//Default
#include "CommanderInputHandler.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UCommanderInputHandler : public UInputHandlerBase
{
	GENERATED_BODY()

public:

	virtual void SetupKeyBindings(class APWPlayerController* InPWPlayerController, class UInputComponent* InputComponent) override;

private:

	void Select_1(const struct FInputActionValue& Value);
	void Select_2(const struct FInputActionValue& Value);
	void Select_3(const struct FInputActionValue& Value);
	void Select_4(const struct FInputActionValue& Value);
	void Select_5(const struct FInputActionValue& Value);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_1;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_2;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_3;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_4;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectAction_5;

	UPROPERTY(Transient)
	class APWPlayerController* PWPlayerController;
};
