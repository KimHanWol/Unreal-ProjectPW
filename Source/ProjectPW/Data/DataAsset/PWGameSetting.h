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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
	float DeathLifeSpan = 3.f;

	//턴 마다 주어지는 행동력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
	float TurnActivePoint = 100.f;

	//최대 플레이어 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
	int32 PlayerCount = 3;

	//이벤트 볼륨이 생기는 확률
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
	float TurnEvent_SpawnVolumeChance = 0.2f;

	//메인 레벨 경로
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "System")
	FName MainLevelPath;

	//BuildableWall 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Object")
	float BuildableWallDefaultHealth = 60.f;
};
