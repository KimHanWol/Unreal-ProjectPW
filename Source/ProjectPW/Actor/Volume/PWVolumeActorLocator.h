// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"

//Default
#include "PWVolumeActorLocator.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActorLocator : public AActor
{
	GENERATED_BODY()
	
public:

	bool IsOccupied() const { return bIsOccupied; }
	void SetIsOccupied(bool bInOccupied);

private:

	// 이미 볼륨이 스폰되어있는지
	UPROPERTY(Transient)
	bool bIsOccupied = false;

};
