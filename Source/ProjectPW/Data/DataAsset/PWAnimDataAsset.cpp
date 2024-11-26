// LINK

//Default
#include "PWAnimDataAsset.h"

//Engine

//Game
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"

void UPWAnimDataAsset::Initialize()
{
	TArray<TSoftObjectPtr<class UAnimMontage>> SoftObjectList;
	AnimationMap.GenerateValueArray(SoftObjectList);

	for (TSoftObjectPtr<class UAnimMontage> SoftObject : SoftObjectList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}
}

const UPWAnimDataAsset* UPWAnimDataAsset::Get(const UObject* WorldContextObj)
{
	UPWGameInstance* PWGameInstance = UPWGameInstance::Get(WorldContextObj);
	if (ensure(IsValid(PWGameInstance) == true))
	{
		return PWGameInstance->GetAnimDataAsset(WorldContextObj);
	}

	return nullptr;
}

TSoftObjectPtr<UAnimMontage> UPWAnimDataAsset::GetAnimMontage(const UObject* WorldContextObj, EAnimMontageType AnimMontageType)
{
	const UPWAnimDataAsset* PWAnimDataAsset = UPWAnimDataAsset::Get(WorldContextObj);
	if (IsValid(PWAnimDataAsset) == false)
	{
		ensure(false);
		return nullptr;
	}

	if (PWAnimDataAsset->AnimationMap.Contains(AnimMontageType) == false)
	{
		ensureMsgf(false, TEXT("AnimDataAsset doesn't have data. check AnimMontageType."));
		return nullptr;
	}

	return PWAnimDataAsset->AnimationMap[AnimMontageType];
}