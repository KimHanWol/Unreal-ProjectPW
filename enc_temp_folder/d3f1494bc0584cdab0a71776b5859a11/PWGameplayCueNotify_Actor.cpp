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

	UE_LOG(LogTemp, Warning, TEXT("%s Cue Executed"), *GetName());

	if (EventType == EGameplayCueEvent::Executed)
	{
		if (IsValid(NiagaraEffectAsset) == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Niagara Asset Played"), *NiagaraEffectAsset->GetName());
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
			UE_LOG(LogTemp, Warning, TEXT("%s Sound Asset Played"), *SoundEffectAsset->GetName());
			UGameplayStatics::PlaySoundAtLocation(MyTarget, SoundEffectAsset, MyTarget->GetActorLocation());
		}
	}
}
