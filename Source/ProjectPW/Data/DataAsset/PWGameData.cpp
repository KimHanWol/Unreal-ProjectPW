// LINK

//Default
#include "PWGameData.h"

//Engine

//Game
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"
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
	TArray<TSoftObjectPtr<UDataTable>> SoftObjectList;
	DataTableMap.GenerateValueArray(SoftObjectList);

	for (TSoftObjectPtr<UDataTable> SoftObject : SoftObjectList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}
}

TSubclassOf<UGameplayEffect> UPWGameData::GetGameplayEffect(const UObject* WorldContextObj, EGameplayEffectType GameplayEffectType)
{
	if (UPWGameData::Get(WorldContextObj)->GameplayEffectMap.Contains(GameplayEffectType) == false)
	{
		ensure(false);
		return nullptr;
	}

	return UPWGameData::Get(WorldContextObj)->GameplayEffectMap[GameplayEffectType];
}