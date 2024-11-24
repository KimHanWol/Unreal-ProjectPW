// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

//Game
#include "PWGameInstance.h"

//Default
#include "PWAssetLoadManager.generated.h"

UCLASS()
class PROJECTPW_API UPWAssetLoadManager : public UObject
{
	GENERATED_BODY()

public:

	static UPWAssetLoadManager* Get(const UObject* WorldContextObject);

	template <typename... Args>
	static void AsyncLoad(const UObject* WorldContextObject, Args... Arguments)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		TArray<TSoftObjectPtr<UObject>> SoftObjectList = { TSoftObjectPtr<UObject>(Arguments)... };
		for (TSoftObjectPtr<UObject> Asset : SoftObjectList)
		{
			if (Asset.IsNull() == true)
			{	
				ensure(false);
				continue;
			}
	
			UPWGameInstance* PWGameInstance = UPWGameInstance::Get(WorldContextObject);
			if (IsValid(PWGameInstance) == true)
			{
				UPWAssetLoadManager* PWAssetLoadManager	= PWGameInstance->GetAssetLoadManager();
				if (ensure(IsValid(PWAssetLoadManager) == true))
				{
					if(PWAssetLoadManager->LoadAssetMap.Contains(Asset.ToSoftObjectPath()) == false)
					{
						TSharedPtr<FStreamableHandle> StreamableHandle = Streamable.RequestAsyncLoad(Asset.ToSoftObjectPath());
						PWAssetLoadManager->StreamableHandleList.Add(StreamableHandle);
						StreamableHandle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([=]()
						{
						    PWAssetLoadManager->LoadAssetMap.Add(TTuple<FSoftObjectPath, UObject*>(Asset.ToSoftObjectPath(), StreamableHandle->GetLoadedAsset()));
							UE_LOG(LogTemp, Warning, TEXT("PWAssetLoadManager : %s asset async loaded."), *Asset.GetAssetName());
							PWAssetLoadManager->StreamableHandleList.Remove(StreamableHandle);
						}));
					}
				}
			}
		}
	}

	template <typename T>
	static T* GetLoadedAsset(const UObject* WorldContextObject, TSoftObjectPtr<UObject> TargetSoftPtr)
	{
		if (TargetSoftPtr.IsNull() == true)
		{
			ensure(false);
			return nullptr;
		}

		const TMap<struct FSoftObjectPath, UObject*> LoadAssetMap = UPWAssetLoadManager::Get(WorldContextObject)->LoadAssetMap;
		if (LoadAssetMap.Contains(TargetSoftPtr.ToSoftObjectPath()) == true)
		{
			T* TargetObject = Cast<T>(LoadAssetMap[TargetSoftPtr.ToSoftObjectPath()]);
			if (IsValid(TargetObject) == true)
			{
				return TargetObject;
			}
		}

		return Cast<T>(TargetSoftPtr.LoadSynchronous());
	}

	void Initialize();

public:

	TArray<TSharedPtr<FStreamableHandle>> StreamableHandleList;

private:

	UPROPERTY(Transient)
	TMap<struct FSoftObjectPath, UObject*> LoadAssetMap;

	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UObject>> SoftObjectListBeforeGameInstanceInit;
	
};
