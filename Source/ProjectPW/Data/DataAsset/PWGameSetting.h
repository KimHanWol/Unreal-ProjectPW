// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

//Game

//Default
#include "PWGameSetting.generated.h"


UCLASS()
class PROJECTPW_API UPWGameSetting: public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWGameSetting* Get(const UObject* WorldContextObj);

public:

	static UPWGameSetting* Instance;

	// 죽었을 때 LifeSpan
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DeathLifeSpan = 3.f;

	//턴 마다 주어지는 행동력
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnActivePoint = 100.f;

	//총 플레이어 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayerCount = 3;
};
