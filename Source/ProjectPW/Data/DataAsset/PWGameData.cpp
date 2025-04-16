// LINK

//Default
#include "PWGameData.h"

//Engine
#include "Engine/DataTable.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
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
	TArray<TSoftObjectPtr<UObject>> AsyncLoadList;

	//Data Table
	if (ensure(IsValid(CharacterDataTable) == true))
	{
		TArray<FPWCharacterDataTableRow*> CharacterDataList;
		CharacterDataTable->GetAllRows<FPWCharacterDataTableRow>(TEXT(""), CharacterDataList);

		for (FPWCharacterDataTableRow* CharacterData : CharacterDataList)
		{
			AsyncLoadList.Add(CharacterData->Portrait);
			AsyncLoadList.Add(CharacterData->Icon);
			AsyncLoadList.Add(CharacterData->Mesh); // TODO: Texture 이나 Material 도 같이 불러와지는 지 확인
			UPWAnimDataAsset* AnimData = CharacterData->AnimDataAsset;
			if (ensure(IsValid(AnimData) == true))
			{
				AnimData->Initialize();
			}
		}
	}

	if (ensure(IsValid(EquipmentDataTable) == true))
	{
		TArray<FPWEquipmentDataTableRow*> EquipmentDataList;
		EquipmentDataTable->GetAllRows< FPWEquipmentDataTableRow>(TEXT(""), EquipmentDataList);

		for (FPWEquipmentDataTableRow* EquipmentData : EquipmentDataList)
		{
			AsyncLoadList.Add(EquipmentData->Icon);
		}
	}

	if (ensure(IsValid(LevelDataTable) == true))
	{
		TArray<FPWLevelDataTableRow*> LevelDataList;
		LevelDataTable->GetAllRows<FPWLevelDataTableRow>(TEXT(""), LevelDataList);

		for (FPWLevelDataTableRow* LevelData : LevelDataList)
		{
			AsyncLoadList.Add(LevelData->LevelImage);
		}
	}

	//Niagara
	TArray<TSoftObjectPtr<UNiagaraSystem>> SoftNiagaraList;
	NiagaraMap.GenerateValueArray(SoftNiagaraList);

	AsyncLoadList.Append(SoftNiagaraList);

	//Material
	AsyncLoadList.Add(SpawnPreviewMI);


	for (TSoftObjectPtr<UObject> SoftObject : AsyncLoadList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}
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

	if (IsValid(PWGameData->LevelDataTable) == false)
	{
		ensure(false);
		return nullptr;
	}

	return PWGameData->LevelDataTable->FindRow<FPWLevelDataTableRow>(LevelDataKey, "");
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

	if (IsValid(PWGameData->EquipmentDataTable) == false)
	{
		ensure(false);
		return nullptr;
	}

	return PWGameData->EquipmentDataTable->FindRow<FPWEquipmentDataTableRow>(EquipmentDataKey, "");
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

const UDataTable* UPWGameData::GetDataTable(EDataTableType DataTableType) const
{
	UDataTable* ResultDataTable = nullptr;
	switch (DataTableType)
	{
	case EDataTableType::None:
		ensure(false);
		break;
	case EDataTableType::Character:
		ResultDataTable = CharacterDataTable;
		break;
	case EDataTableType::Level:
		ResultDataTable = LevelDataTable;
		break;
	case EDataTableType::Equipment:
		ResultDataTable = EquipmentDataTable;
		break;
	default:
		ensure(false);
		break;
	}

	return ResultDataTable;
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