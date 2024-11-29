// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Volume/PWVolumeActorBase.h"

//Default
#include "PWVolumeActor_Effect.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActor_Effect : public APWVolumeActorBase
{
	GENERATED_BODY()

protected:

	virtual void Execute_Internal(AActor* OverlappedActor) override;

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<class UGameplayEffect>, struct FSetByCallerData> VolumeEffectDataMap;
};
