// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Volume/PWVolumeActorBase.h"

//Default
#include "PWVolumeActorLocator.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActorLocator : public APWVolumeActorBase
{
	GENERATED_BODY()

public:

	bool IsSpawnable();

	void SetIsOccupied(bool bInOccupied);
	bool IsOccupied() const { return bIsOccupied; }

private:

	// 이미 볼륨이 스폰되어있는지
	UPROPERTY(Transient)
	bool bIsOccupied = false;

};
