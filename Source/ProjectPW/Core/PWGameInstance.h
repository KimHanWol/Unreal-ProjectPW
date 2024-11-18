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

	static class UPWEventManager* GetEventManager(const UObject* WorldContextObj);
	FORCEINLINE class UPWEventManager* GetEventManager() const { return PWEventManager; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<class UPWGameData> PWGameDataPtr;

private:

	UPROPERTY(Transient)
	class UPWGameData* PWGameData;

	UPROPERTY(Transient)
	class UPWEventManager* PWEventManager;
};
