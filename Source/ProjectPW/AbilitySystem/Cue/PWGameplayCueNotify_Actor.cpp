// LINK

//Default
#include "PWGameplayCueNotify_Actor.h"

//Engine
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//Game
#include "Core/PWAssetLoadManager.h"

void APWGameplayCueNotify_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	if (EventType == EGameplayCueEvent::Executed)
	{
		if (IsValid(NiagaraEffectAsset) == true)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				NiagaraEffectAsset,
				GetActorTransform().GetLocation(),
				GetActorTransform().GetRotation().Rotator(),
				GetActorTransform().GetScale3D(),
				true,
				true,
				ENCPoolMethod::None,
				true
			);
		}

		if (IsValid(SoundEffectAsset) == true)
		{
			UGameplayStatics::PlaySoundAtLocation(MyTarget, SoundEffectAsset, MyTarget->GetActorLocation());
		}
	}
}
