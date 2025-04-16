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

	// 어차피 즉시 사용되는 에셋이라 하드 레퍼런스로 가지고 있음
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraSystem* NiagaraEffectAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* SoundEffectAsset;
};
