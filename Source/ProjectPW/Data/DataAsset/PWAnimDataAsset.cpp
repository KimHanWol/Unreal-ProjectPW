// LINK

//Default
#include "PWAnimDataAsset.h"

//Engine

//Game
#include "Core/PWGameInstance.h"

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
	//TODO: const 전부 확인하기
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