// LINK

//Default
#include "PWAnimDataAsset.h"

//Engine

//Game
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "PWGameData.h"

void UPWAnimDataAsset::Initialize()
{
	TArray<TSoftObjectPtr<class UAnimMontage>> MontageSoftObjectList;
	MontageMap.GenerateValueArray(MontageSoftObjectList);

	for (TSoftObjectPtr<class UAnimMontage> SoftObject : MontageSoftObjectList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}

	TArray<TSoftObjectPtr<class UAnimationAsset>> AnimationSoftObjectList;
	AnimationMap.GenerateValueArray(AnimationSoftObjectList);

	for (TSoftObjectPtr<class UAnimationAsset> SoftObject : AnimationSoftObjectList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}

	UPWAssetLoadManager::AsyncLoad(this, BlendSpace);
}

const UPWAnimDataAsset* UPWAnimDataAsset::Get(const UObject* WorldContextObj, ECharacterType CharcterType)
{
	const FPWCharacterDataTableRow* CharacterData = UPWGameData::FindCharacterTableRow(WorldContextObj, CharcterType);
	if (CharacterData == nullptr)
	{
		ensure(false);
		return nullptr;
	}

	return CharacterData->AnimDataAsset;
}

TSoftObjectPtr<UAnimMontage> UPWAnimDataAsset::GetAnimMontage(const UObject* WorldContextObj, ECharacterType CharcterType, EAnimMontageType AnimMontageType)
{
	const UPWAnimDataAsset* PWAnimDataAsset = UPWAnimDataAsset::Get(WorldContextObj, CharcterType);
	if (IsValid(PWAnimDataAsset) == false)
	{
		ensure(false);
		return nullptr;
	}

	if (PWAnimDataAsset->MontageMap.Contains(AnimMontageType) == false)
	{
		ensureMsgf(false, TEXT("AnimDataAsset doesn't have data. check AnimMontageType."));
		return nullptr;
	}

	return PWAnimDataAsset->MontageMap[AnimMontageType];
}

TSoftObjectPtr<UAnimationAsset> UPWAnimDataAsset::GetAnimation(const UObject* WorldContextObj, ECharacterType CharcterType, EAnimationType AnimationType)
{
	const UPWAnimDataAsset* PWAnimDataAsset = UPWAnimDataAsset::Get(WorldContextObj, CharcterType);
	if (IsValid(PWAnimDataAsset) == false)
	{
		ensure(false);
		return nullptr;
	}

	if (PWAnimDataAsset->AnimationMap.Contains(AnimationType) == false)
	{
		ensureMsgf(false, TEXT("AnimDataAsset doesn't have data. check AnimationType."));
		return nullptr;
	}

	return PWAnimDataAsset->AnimationMap[AnimationType];
}

TSoftObjectPtr<UBlendSpace> UPWAnimDataAsset::GetBlendSpace(const UObject* WorldContextObj, ECharacterType CharcterType)
{
	const UPWAnimDataAsset* PWAnimDataAsset = UPWAnimDataAsset::Get(WorldContextObj, CharcterType);
	if (IsValid(PWAnimDataAsset) == false)
	{
		ensure(false);
		return nullptr;
	}

	return PWAnimDataAsset->BlendSpace;
}
