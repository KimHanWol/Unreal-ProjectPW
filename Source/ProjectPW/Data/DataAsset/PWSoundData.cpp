// LINK

//Default
#include "PWSoundData.h"

//Engine
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"

//Game
#include "Core/PWAssetLoadManager.h"
#include "Core/PWGameInstance.h"

UPWSoundData* UPWSoundData::Instance = nullptr;

const UPWSoundData* UPWSoundData::Get(const UObject* WorldContextObj)
{
	const UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
	if (ensure(IsValid(PWGameInst) == true))
	{
		return PWGameInst->GetSoundData();
	}

	return nullptr;
}

void UPWSoundData::Initialize()
{
	TArray<TSoftObjectPtr<class USoundBase>> SoundList;

    // 프로퍼티 메타데이터 가져오기
    for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        // SoftObjectPtr인지 확인
        if (FSoftObjectProperty* SoftObjProp = CastField<FSoftObjectProperty>(Property))
        {
            UClass* PropertyClass = SoftObjProp->PropertyClass;

            // SoundBase 기반인지 확인
            if (PropertyClass && PropertyClass->IsChildOf(USoundBase::StaticClass()))
            {
                // 프로퍼티 값 가져오기
                FSoftObjectPtr Value = SoftObjProp->GetPropertyValue_InContainer(this);
                if (ensure(Value.IsNull() == false))
                {
                    FSoftObjectPath Path = Value.ToSoftObjectPath();

                    if (Path.IsNull() == false)
                    {
                        SoundList.Add(TSoftObjectPtr<USoundBase>(Path));
                    }
                }
            }
        }
    }

	for (TSoftObjectPtr<USoundBase> SoftObject : SoundList)
	{
		UPWAssetLoadManager::AsyncLoad(this, SoftObject);
	}
}