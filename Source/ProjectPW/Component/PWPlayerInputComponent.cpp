// LINK

//Default
#include "PWPlayerInputComponent.h"

//Engine
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/InputHandler/CharacterInputHandler.h"
#include "Core/InputHandler/CommanderInputHandler.h"
#include "Core/PWEventManager.h"

void UPWPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	PWPlayerController = Cast<APWPlayerController>(GetOwner());
	ensure(PWPlayerController);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UPWPlayerInputComponent::OnPlayerPossessed);
	}

	//BeginPlay 이전에 이미 빙의되어 있는 경우
	if (IsValid(PWPlayerController) == true)
	{
		if (IsValid(PWPlayerController->GetPawn()) == true)
		{
			OnPlayerPossessed(PWPlayerController->GetPawn(), true); //처음 초기화 시 이므로 항상 커맨더
		}
	}
}

void UPWPlayerInputComponent::InitMappingContext()
{
	TryCreateInputHandler();

	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	if (IsValid(DefaultContext))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PWPlayerController->GetLocalPlayer());
		if (IsValid(EnhancedInputSubsys) == true)
		{
			EnhancedInputSubsys->AddMappingContext(DefaultContext, 0);
		}
	}
}

void UPWPlayerInputComponent::SetInputEnabled(bool EnableCommanderInput, bool EnableCharacterInput)
{
	TryCreateInputHandler();

	if (IsValid(CommanderInputHandler) == true)
	{
		CommanderInputHandler->SetInputEnabled(EnableCommanderInput);
	}

	if(IsValid(CharacterInputHandler) == true)
	{
		CharacterInputHandler->SetInputEnabled(EnableCharacterInput);
	}
}

void UPWPlayerInputComponent::SetMouseInputToGameAndUI(bool bInShowWithCursor)
{
	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	PWPlayerController->bShowMouseCursor = bInShowWithCursor;

	PWPlayerController->FlushPressedKeys();

	FInputModeGameAndUI InputMode;
	PWPlayerController->SetInputMode(InputMode);
}

void UPWPlayerInputComponent::SetMouseInputToGame()
{
	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	PWPlayerController->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PWPlayerController->SetInputMode(InputMode);
}

void UPWPlayerInputComponent::TryCreateInputHandler()
{
	if (IsValid(CommanderInputHandler) == true &&
		IsValid(CharacterInputHandler) == true)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	if (IsValid(PWPlayerController->InputComponent) == true)
	{
		if (IsValid(CommanderInputHandlerClass) == true)
		{
			CommanderInputHandler = NewObject<UCommanderInputHandler>(this, CommanderInputHandlerClass);
			CommanderInputHandler->SetupKeyBindings(PWPlayerController, PWPlayerController->InputComponent);
			CommanderInputHandler->SetInputEnabled(false);
		}

		if (IsValid(CharacterInputHandlerClass) == true)
		{
			CharacterInputHandler = NewObject<UCharacterInputHandler>(this, CharacterInputHandlerClass);
			CharacterInputHandler->SetupKeyBindings(PWPlayerController, PWPlayerController->InputComponent);
			CharacterInputHandler->SetInputEnabled(false);
		}
	}
}

void UPWPlayerInputComponent::OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander)
{
	if (bIsCommander == true)
	{
		SetMouseInputToGameAndUI(true);
	}
	else
	{
		SetMouseInputToGame();
	}
}
