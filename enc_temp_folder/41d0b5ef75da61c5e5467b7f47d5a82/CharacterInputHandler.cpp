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
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Look);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UCharacterInputHandler::Fire);
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
	const FRotator Rotation = PWPlayerController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (FMath::IsNearlyZero(Value.Get<FVector2D>().X) == false)
	{
		PWPlayerCharacter->AddMovementInput(FowardDirection * Value.Get<FVector2D>().X);
	}

	if (FMath::IsNearlyZero(Value.Get<FVector2D>().Y) == false)
	{
		PWPlayerCharacter->AddMovementInput(SideDirection * Value.Get<FVector2D>().Y);
	}
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

void UCharacterInputHandler::Fire(const FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}
}