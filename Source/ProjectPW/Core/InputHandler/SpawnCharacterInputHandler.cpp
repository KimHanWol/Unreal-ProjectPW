// LINK

//Default
#include "SpawnCharacterInputHandler.h"

//Engine
#include "EnhancedInputComponent.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"

void USpawnCharacterInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController)
{
	Super::SetupKeyBindings(InPWPlayerController);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InPWPlayerController->InputComponent);
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
	}
}

void USpawnCharacterInputHandler::TrySpawn(const struct FInputActionValue& Value)
{
	
}