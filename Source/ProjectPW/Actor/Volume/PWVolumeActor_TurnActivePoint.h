// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Volume/PWVolumeActorBase.h"

//Default
#include "PWVolumeActor_TurnActivePoint.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActor_TurnActivePoint : public APWVolumeActorBase
{
	GENERATED_BODY()
	
protected:

	virtual void Execute_Internal(AActor* OverlappedActor) override;

private:

	//추가 되는 행동력
	UPROPERTY(EditAnywhere)
	float AddableTurnActivePoint;
};
