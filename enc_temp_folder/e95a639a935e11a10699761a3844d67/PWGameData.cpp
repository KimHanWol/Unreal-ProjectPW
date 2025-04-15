// LINK

//Default
#include "PWGameData.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataTable/PWEquipmentDataTableRow.h"
#include "Data/DataTable/PWLevelDataTableRow.h"
#include "Helper/PWGameplayStatics.h"

UPWGameData* UPWGameData::Instance = nullptr;

const UPWGameData* UPWGameData::Get(const UObject* WorldContextObj)
{
	const UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
	if (ensure(IsValid(PWGameInst) == true))
	{
		return PWGameInst->GetGameData();
	}

	return nullptr;
}

void UPWGameData::Initialize()
{
	//Data Table
	TArray<TSoftObjectPtr<UDataTable>> SoftDataTableList;
	DataTableMap.GenerateValueArray(SoftDataTableList);

	for (TSoftObjectPtr<UDataTable> SoftObject : SoftDataTableList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}

	//Niagara
	TArray<TSoftObjectPtr<UNiagaraSystem>> SoftNiagaraList;
	NiagaraMap.GenerateValueArray(SoftNiagaraList);

	for (TSoftObjectPtr<UNiagaraSystem> SoftObject : SoftNiagaraList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}

	UPWAssetLoadManager::AsyncLoad(this, SpawnPreviewMI);
}

TSubclassOf<UGameplayEffect> UPWGameData::GetGameplayEffect(const UObject* WorldContextObj, EGameplayEffectType GameplayEffectType)
{
	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	if (PWGameData->GameplayEffectMap.Contains(GameplayEffectType) == false)
	{
		ensure(false);
		return nullptr;
	}

	return UPWGameData::Get(WorldContextObj)->GameplayEffectMap[GameplayEffectType];
}

const FPWCharacterDataTableRow* UPWGameData::FindCharacterTableRow(const UObject* WorldContextObj, ECharacterType CharacterType)
{
	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	const FPWCharacterDataTableRow* CharacterData = nullptr;
	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	for (const FPWCharacterDataTableRow* InCharacterData : CharacterDataList)
	{
		if (InCharacterData != nullptr && InCharacterData->CharacterType == CharacterType)
		{
			return CharacterData = InCharacterData;
		}
	}

	return nullptr;
}

const FPWLevelDataTableRow* UPWGameData::FindLevelTableRow(const UObject* WorldContextObj, FName LevelDataKey)
{
	if (LevelDataKey.IsNone() == true)
	{
		ensure(false);
		return nullptr;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	TSoftObjectPtr<class UDataTable> LevelDataTable = PWGameData->DataTableMap[EDataTableType::Level];
	if (LevelDataTable.IsNull() == true)
	{
		ensure(false);
		return nullptr;
	}

	return LevelDataTable.LoadSynchronous()->FindRow<FPWLevelDataTableRow>(LevelDataKey, "");
}

const FPWEquipmentDataTableRow* UPWGameData::FindEquipmentTableRow(const UObject* WorldContextObj, FName EquipmentDataKey)
{
	if (EquipmentDataKey.IsNone() == true)
	{
		ensure(false);
		return nullptr;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	TSoftObjectPtr<class UDataTable> EquipmentDataTable = PWGameData->DataTableMap[EDataTableType::Equipment];
	if (EquipmentDataTable.IsNull() == true)
	{
		ensure(false);
		return nullptr;
	}

	return EquipmentDataTable.LoadSynchronous()->FindRow<FPWEquipmentDataTableRow>(EquipmentDataKey, "");
}

TSubclassOf<APWVolumeActorBase> UPWGameData::GetVolumeActorRandom(const UObject* WorldContextObj)
{
	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	int32 VolumeActorListNum = PWGameData->VolumeActorList.Num();
	if (VolumeActorListNum <= 0)
	{
		ensure(false);
		return nullptr;
	}

	int32 RandIndex = FMath::RandRange(0, VolumeActorListNum - 1);
	return UPWGameData::Get(WorldContextObj)->VolumeActorList[RandIndex];
}

const TSoftObjectPtr<UDataTable> UPWGameData::GetDataTable(EDataTableType DataTableType) const
{
	if (DataTableMap.Contains(DataTableType) == false)
	{
		ensure(false);
		return TSoftObjectPtr<UDataTable>();
	}

	return DataTableMap[DataTableType];
}

const TSoftObjectPtr<class UNiagaraSystem> UPWGameData::GetNiagara(const UObject* WorldContextObj, const FGameplayTag& GameplayTag)
{
	const UPWGameData* PWGameData = UPWGameData::Get(WorldContextObj);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	if (PWGameData->NiagaraMap.Contains(GameplayTag) == false)
	{
		ensure(false);
		return TSoftObjectPtr<UNiagaraSystem>();
	}

	return PWGameData->NiagaraMap[GameplayTag];
}