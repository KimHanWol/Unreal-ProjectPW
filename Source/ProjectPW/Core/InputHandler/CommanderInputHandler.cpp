// LINK

//Default
#include "CommanderInputHandler.h"

//Engine
#include "EnhancedInputComponent.h"

//Game
#include "Actor/Character/PWPlayerController.h"

void UCommanderInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController, UInputComponent* InputComponent)
{
	Super::SetupKeyBindings(InPWPlayerController, InputComponent);

	if (IsValid(InputComponent) == false)
	{
		return;
	}

	PWPlayerController = InPWPlayerController;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (IsValid(EnhancedInputComponent) == false)
	{
		return;
	}

	EnhancedInputComponent->BindAction(SelectAction_1, ETriggerEvent::Triggered, this, &UCommanderInputHandler::Select_1);
	EnhancedInputComponent->BindAction(SelectAction_2, ETriggerEvent::Triggered, this, &UCommanderInputHandler::Select_2);
	EnhancedInputComponent->BindAction(SelectAction_3, ETriggerEvent::Triggered, this, &UCommanderInputHandler::Select_3);
	EnhancedInputComponent->BindAction(SelectAction_4, ETriggerEvent::Triggered, this, &UCommanderInputHandler::Select_4);
	EnhancedInputComponent->BindAction(SelectAction_5, ETriggerEvent::Triggered, this, &UCommanderInputHandler::Select_5);
}

void UCommanderInputHandler::Select_1(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerController->SelectCharacter(1);
}

void UCommanderInputHandler::Select_2(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerController->SelectCharacter(2);
}

void UCommanderInputHandler::Select_3(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerController->SelectCharacter(3);
}

void UCommanderInputHandler::Select_4(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerController->SelectCharacter(4);
}

void UCommanderInputHandler::Select_5(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerController->SelectCharacter(5);
}