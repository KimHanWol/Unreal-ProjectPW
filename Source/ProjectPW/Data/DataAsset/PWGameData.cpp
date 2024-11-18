// LINK

//Default
#include "PWGameData.h"

//Engine

//Game
#include "Core/PWGameInstance.h"
#include "Helper/PWGameplayStatics.h"

UPWGameData* UPWGameData::Instance = nullptr;

const UPWGameData* UPWGameData::Get(const UObject* WorldContextObj)
{
	UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
	check(PWGameInst);

	return PWGameInst->GetGameData();
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

void UPWGameData::PostLoad()
{
	Super::PostLoad();

	TArray<FSoftObjectPath> ItemsToStream;
	for (TTuple <TEnumAsByte<EGameplayEffectType>, TSoftObjectPtr<UDataTable>> DataTableData : DataTableMap)
	{
		if (DataTableData.Value.IsNull() == false)
		{
			ItemsToStream.AddUnique(DataTableData.Value.ToSoftObjectPath());
		}
	}

	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);
}
