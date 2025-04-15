// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"

//Game

//Default
#include "PWGameplayCueNotify_Actor.generated.h"

UCLASS()
class PROJECTPW_API APWGameplayCueNotify_Actor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

protected:

	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UNiagaraSystem> NiagaraEffectAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USoundBase> SoundEffectAsset;
};
