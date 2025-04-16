// LINK

//Default
#include "CharacterInputHandler.h"

//Engine
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputTriggers.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Helper/PWGameplayStatics.h"

void UCharacterInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController)
{
	Super::SetupKeyBindings(InPWPlayerController);

	PWPlayerController = InPWPlayerController;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PWPlayerController->InputComponent);
	if (IsValid(EnhancedInputComponent) == false)
	{
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Jump);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Look);
	EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::LoadPrevState);
	EnhancedInputComponent->BindAction(ExecuteAction_Main, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Execute_Main_Triggered);
	EnhancedInputComponent->BindAction(ExecuteAction_Main, ETriggerEvent::Completed, this, &UCharacterInputHandler::Execute_Main_Completed);
	EnhancedInputComponent->BindAction(ExecuteAction_Sub, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Execute_Sub_Triggered);
	EnhancedInputComponent->BindAction(ExecuteAction_Sub, ETriggerEvent::Completed, this, &UCharacterInputHandler::Execute_Sub_Completed);
	EnhancedInputComponent->BindAction(SelectAction_ESC, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Select_ESC);
}

void UCharacterInputHandler::Move(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	// 방향
	const FRotator& Rotation = PWPlayerController->GetControlRotation();
	const FRotator& YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector& FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector& SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (FMath::IsNearlyZero(Value.Get<FVector2D>().Y) == false)
	{
		PWPlayerCharacter->AddMovementInput(FowardDirection * Value.Get<FVector2D>().Y);
	}

	if (FMath::IsNearlyZero(Value.Get<FVector2D>().X) == false)
	{
		PWPlayerCharacter->AddMovementInput(SideDirection * Value.Get<FVector2D>().X);
	}
}

void UCharacterInputHandler::Jump(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->Jump();
}

void UCharacterInputHandler::Look(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->AddControllerYawInput(Value.Get<FVector2D>().X);
	PWPlayerCharacter->AddControllerPitchInput(Value.Get<FVector2D>().Y);
}

void UCharacterInputHandler::LoadPrevState(const FInputActionValue& Value)
{
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->CS_LoadPrevSnapshot();
	}
}

void UCharacterInputHandler::Execute_Main_Triggered(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->Execute_Main_Triggered();
}

void UCharacterInputHandler::Execute_Main_Completed(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->Execute_Main_Completed();
}

void UCharacterInputHandler::Execute_Sub_Triggered(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->Execute_Sub_Triggered();
}

void UCharacterInputHandler::Execute_Sub_Completed(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(PWPlayerController->GetPawn());
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	PWPlayerCharacter->Execute_Sub_Completed();
}

void UCharacterInputHandler::Select_ESC(const FInputActionValue& Value)
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(0);
	}
}
