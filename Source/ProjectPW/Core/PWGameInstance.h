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

	static class UPWAssetLoadManager* GetAssetLoadManager(const UObject* WorldContextObj);
	FORCEINLINE class UPWAssetLoadManager* GetAssetLoadManager() const { return PWAssetLoadManager; }

protected:

	//내부에서 또 비동기 로딩을 해야 하고 Init 시점에 초기화 하기 때문에
	//여기서 참조하는 에셋들은 하드 레퍼런스로 가지기.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPWGameSetting* PWGameSetting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPWGameData* PWGameData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UPWAnimDataAsset* PWAnimDataAsset;

private:

	UPROPERTY(Transient)
	class UPWEventManager* PWEventManager;

	UPROPERTY(Transient)
	class UPWAssetLoadManager* PWAssetLoadManager;
};
