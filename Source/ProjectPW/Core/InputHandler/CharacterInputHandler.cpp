// LINK

//Default
#include "CharacterInputHandler.h"

//Engine
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputTriggers.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"

void UCharacterInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController, UInputComponent* InputComponent)
{
	Super::SetupKeyBindings(InPWPlayerController, InputComponent);

	PWPlayerController = InPWPlayerController;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (IsValid(EnhancedInputComponent) == false)
	{
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Jump);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Look);
	EnhancedInputComponent->BindAction(ExecuteAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Execute);
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

void UCharacterInputHandler::Execute(const FInputActionValue& Value)
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

	PWPlayerCharacter->Execute();
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

	PWPlayerController->SelectCharacter(0);
}
