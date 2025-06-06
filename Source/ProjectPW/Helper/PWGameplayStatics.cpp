﻿// LINK

//Default
#include "PWGameplayStatics.h"

//Engine
#include "EngineUtils.h"
#include "GameFramework/Character.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWGameInstance.h"
#include "Core/PWGameState.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

APWPlayerController* UPWGameplayStatics::GetLocalPlayerController(const UObject* WorldContextObj)
{
	if (IsValid(WorldContextObj) == false)
	{
		ensure(false);
		return nullptr;
	}

	return Cast<APWPlayerController>(GetPlayerController(WorldContextObj, 0));
}

APWPlayerController* UPWGameplayStatics::GetOtherPlayerController(class APWPlayerController* CurrentPlayerController)
{
	//Server Only
	if (IsValid(CurrentPlayerController) == false || CurrentPlayerController->HasAuthority() == false)
	{
		return nullptr;
	}

	if (IsValid(CurrentPlayerController) == false)
	{
		return nullptr;
	}

	UWorld* World = CurrentPlayerController->GetWorld();

	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true && PWPlayerController != CurrentPlayerController)
			{
				return PWPlayerController;
			}
		}
	}
	return nullptr;
}

TArray<APWPlayerController*> UPWGameplayStatics::GetAllPlayerController(const UObject* WorldContextObj)
{
	TArray<APWPlayerController*> AllPlayerControllerList;

	UWorld* World = WorldContextObj->GetWorld();
	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
		{
			APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Iterator->Get());
			if (IsValid(PWPlayerController) == true)
			{
				AllPlayerControllerList.Add(PWPlayerController);
			}
		}
	}

	return AllPlayerControllerList;
}

TArray<class APWPlayerCharacter*> UPWGameplayStatics::GetAllPlayerCharacter(const UObject* WorldContextObj)
{
	TArray<APWPlayerCharacter*> PlayerCharacterList;

	UWorld* World = WorldContextObj->GetWorld();
	if (IsValid(World))
	{
		for (TActorIterator<APWPlayerCharacter> ActorItr(World); ActorItr; ++ActorItr)
		{		
			APWPlayerCharacter* PWPlayerController = Cast<APWPlayerCharacter>(*ActorItr);
			if (IsValid(PWPlayerController) == true)
			{
				PlayerCharacterList.Add(PWPlayerController);
			}
		}
	}
	
	return PlayerCharacterList;
}

APWPlayerState* UPWGameplayStatics::GetLocalPlayerState(const UObject* WorldContextObj)
{
	APWPlayerState* PWPlayerState = nullptr;

	APWPlayerController* PWPlayerController = GetLocalPlayerController(WorldContextObj);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true)
		{
			return PWPlayerState;
		}
	}

	return nullptr;
}

FName UPWGameplayStatics::GetSelectedLevelKey(const UObject* WorldContextObj)
{
	FName SelectedInGameLevelKey = UPWGameInstance::Get(WorldContextObj)->SelectedInGameLevelKey;

#if WITH_EDITOR
	// 에디터에서 DevMap 바로 열었을 때
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(WorldContextObj);
	if (IsValid(PWGameSetting) == true)
	{
		if (SelectedInGameLevelKey.IsNone() == true)
		{
			SelectedInGameLevelKey = PWGameSetting->InGameDevMapName;
		}
	}
#endif // WITH_EDITOR

	return SelectedInGameLevelKey;
}

ETeamSide UPWGameplayStatics::GetLocalPlayerTeamSide(const UObject* WorldContextObj)
{
	APWPlayerController* LocalPlayerController = GetLocalPlayerController(WorldContextObj);
	if (IsValid(LocalPlayerController) == true)
	{
		return LocalPlayerController->GetTeamSide();
	}

	ensure(false);
	return ETeamSide::Red;
}

const FPWCharacterDataTableRow* UPWGameplayStatics::FindCharacterData(const UObject* WorldContextObj, const ECharacterType TargetCharacterType)
{
	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = UPWGameData::Get(WorldContextObj)->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	for (const FPWCharacterDataTableRow* CharacterData : CharacterDataList)
	{
		if (CharacterData->CharacterType == TargetCharacterType)
		{
			return CharacterData;
		}
	}

	return nullptr;
}

const FPWEquipmentDataTableRow* UPWGameplayStatics::GetEquipmentData(const UObject* WorldContextObj, FName EquipmentKey)
{
	return UPWGameData::Get(WorldContextObj)->FindEquipmentTableRow(WorldContextObj, EquipmentKey);
}

const FPWEquipmentDataTableRow* UPWGameplayStatics::FindEquipmentData(const UObject* WorldContextObj, const ECharacterType TargetCharacterType)
{
	const FPWCharacterDataTableRow* CharacterData = UPWGameplayStatics::FindCharacterData(WorldContextObj, TargetCharacterType);
	if (CharacterData == nullptr)
	{
		ensure(false);
		return nullptr;
	}

	return GetEquipmentData(WorldContextObj, CharacterData->EquipmentKey);
}
