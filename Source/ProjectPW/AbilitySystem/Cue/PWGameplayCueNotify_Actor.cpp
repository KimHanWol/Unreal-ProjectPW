// LINK

//Default
#include "PWGameplayCueNotify_Actor.h"

//Engine
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//Game
#include "Core/PWAssetLoadManager.h"


void APWGameplayCueNotify_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	if (EventType == EGameplayCueEvent::Executed)
	{
		if (ensure(NiagaraEffectAsset.IsNull() == false))
		{
			UNiagaraSystem* NiagaraEffect = NiagaraEffectAsset.LoadSynchronous();
			if (ensure(IsValid(NiagaraEffect) == true))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					NiagaraEffect,
					GetActorTransform().GetLocation(),
					GetActorTransform().GetRotation().Rotator(),
					GetActorTransform().GetScale3D(),
					true,
					true,
					ENCPoolMethod::None,
					true
				);
			}
		}
	}
}
