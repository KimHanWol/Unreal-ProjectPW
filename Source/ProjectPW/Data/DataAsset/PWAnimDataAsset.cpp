// LINK

//Default
#include "PWAnimDataAsset.h"

//Engine

//Game
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"

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

	if (PWAnimDataAsset->MontageMap.Contains(AnimMontageType) == false)
	{
		ensureMsgf(false, TEXT("AnimDataAsset doesn't have data. check AnimMontageType."));
		return nullptr;
	}

	return PWAnimDataAsset->MontageMap[AnimMontageType];
}

TSoftObjectPtr<UAnimationAsset> UPWAnimDataAsset::GetAnimation(const UObject* WorldContextObj, EAnimationType AnimationType)
{
	const UPWAnimDataAsset* PWAnimDataAsset = UPWAnimDataAsset::Get(WorldContextObj);
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
