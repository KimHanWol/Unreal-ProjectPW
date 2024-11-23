// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/GameInstance.h"

//Default
#include "PWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UPWGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

public:

	static UPWGameInstance* Get(const UObject* WorldContextObj);

	static class UPWGameData* GetGameData(const UObject* WorldContextObj);
	class UPWGameData* GetGameData();

	static class UPWGameSetting* GetGameSetting(const UObject* WorldContextObj);
	class UPWGameSetting* GetGameSetting();

	static class UPWAnimDataAsset* GetAnimDataAsset(const UObject* WorldContextObj);
	class UPWAnimDataAsset* GetAnimDataAsset();

	static class UPWEventManager* GetEventManager(const UObject* WorldContextObj);
	FORCEINLINE class UPWEventManager* GetEventManager() const { return PWEventManager; }
	//TODO: Get 함수들 FORCEINLINE 고려해보기

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UPWGameData> PWGameDataPtr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UPWGameSetting> PWGameSettingPtr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UPWAnimDataAsset> PWAnimDataAssetPtr;

private:

	UPROPERTY(Transient)
	class UPWEventManager* PWEventManager;
};
