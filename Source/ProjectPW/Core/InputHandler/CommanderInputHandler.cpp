﻿// LINK

//Default
#include "CommanderInputHandler.h"

//Engine
#include "EnhancedInputComponent.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"

void UCommanderInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController)
{
	Super::SetupKeyBindings(InPWPlayerController);

	PWPlayerController = InPWPlayerController;
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PWPlayerController->InputComponent);
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(1);
	}
}

void UCommanderInputHandler::Select_2(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(2);
	}
}

void UCommanderInputHandler::Select_3(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(3);
	}
}

void UCommanderInputHandler::Select_4(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(4);
	}
}

void UCommanderInputHandler::Select_5(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(5);
	}
}