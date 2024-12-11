// LINK

//Default
#include "SpawnCharacterInputHandler.h"

//Engine
#include "EnhancedInputComponent.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/Object/PWSpawnAreaActor.h"
#include "Core/PWPlayerState.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"

void USpawnCharacterInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController)
{
	Super::SetupKeyBindings(InPWPlayerController);

	if (IsValid(InPWPlayerController) == false)
	{
		ensure(false);
		return;
	}
	PWPlayerController = InPWPlayerController;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PWPlayerController->InputComponent);
	if (IsValid(EnhancedInputComponent) == false)
	{
		return;
	}

	EnhancedInputComponent->BindAction(SelectCharacter_1, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_1);
	EnhancedInputComponent->BindAction(SelectCharacter_2, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_2);
	EnhancedInputComponent->BindAction(SelectCharacter_3, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_3);
	EnhancedInputComponent->BindAction(SpawnAction, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::TrySpawn);
}

void USpawnCharacterInputHandler::Select_1(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	if (ensure(CharacterDataList.Num() > 0))
	{
		SelectedCharacterType = CharacterDataList[0]->CharacterType;
		SelectedCharacterTypeChangedDelegate.Broadcast(SelectedCharacterType);
	}

}

void USpawnCharacterInputHandler::Select_2(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	if (ensure(CharacterDataList.Num() > 1))
	{
		SelectedCharacterType = CharacterDataList[1]->CharacterType;
		SelectedCharacterTypeChangedDelegate.Broadcast(SelectedCharacterType);
	}

}

void USpawnCharacterInputHandler::Select_3(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	if (ensure(CharacterDataList.Num() > 2))
	{
		SelectedCharacterType = CharacterDataList[2]->CharacterType;
		SelectedCharacterTypeChangedDelegate.Broadcast(SelectedCharacterType);
	}
}

void USpawnCharacterInputHandler::TrySpawn(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (SpawnedCharacterNum >= 5)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerState* PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

    FVector WorldLocation;
	FVector WorldDirection;

	float ScreenX;
	float ScreenY;

    if (PWPlayerController->GetMousePosition(ScreenX, ScreenY))
    {
        PWPlayerController->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection);

        const FVector& StartLostion = WorldLocation;
        const FVector& EndLocation = WorldLocation + (WorldDirection * 10000.0f);

        FHitResult HitResult;
		bool bIsHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, StartLostion, EndLocation, ECC_Visibility);

		bool bSpawnAreaFound = false;
		APWSpawnAreaActor* PWSpawnAreaActor = Cast<APWSpawnAreaActor>(HitResult.GetActor());
		if (IsValid(PWSpawnAreaActor) == true)
		{
			if (PWSpawnAreaActor->GetTeamSide() == PWPlayerState->GetTeamSide())
			{
				bSpawnAreaFound = true;
			}
		}

        if (bIsHitSuccess == true && bSpawnAreaFound == true)
        {
			APawn* CommanderPawn = PWPlayerState->GetCommanderPawn();
			if (IsValid(CommanderPawn) == true)
			{
				FRotator CommanderPawnRotator = CommanderPawn->GetActorRotation();
				FTransform Transform;
				Transform.SetLocation(HitResult.ImpactPoint);
				Transform.SetRotation(FQuat(CommanderPawnRotator));

				PWPlayerController->CS_SpawnCharacter(PWPlayerController, SelectedCharacterType, Transform);
				SpawnedCharacterNum++;
			}
        }
    }
}