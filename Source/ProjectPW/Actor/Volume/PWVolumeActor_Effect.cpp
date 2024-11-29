// LINK

//Default
#include "PWVolumeActor_Effect.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"

void APWVolumeActor_Effect::Execute_Internal(AActor* OverlappedActor)
{
	if (IsValid(OverlappedActor) == false || OverlappedActor->HasAuthority() == false)
	{
		return;
	}

	FString LogString;
	LogString += TEXT("VOLUME ACTIVATED\n");
	for (const TTuple<TSubclassOf<UGameplayEffect>, FSetByCallerData>& VolumeEffectData : VolumeEffectDataMap)
	{
		TSubclassOf<UGameplayEffect> GameplayEffectClass = VolumeEffectData.Key;
		if (IsValid(GameplayEffectClass) == false)
		{
			ensure(false);
			continue;
		}

		APWPlayerCharacter* OverlappedCharacter = Cast<APWPlayerCharacter>(OverlappedActor);
		if (IsValid(OverlappedCharacter) == false)
		{
			return;
		}

		UAbilitySystemComponent* TargetASC = OverlappedCharacter->GetComponentByClass<UAbilitySystemComponent>();
		if (IsValid(TargetASC) == false)
		{
			ensure(false);
			continue;
		}

		FGameplayEffectContextHandle GameplayEffectContextHandle;
		GameplayEffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass.Get(), 1.0f, GameplayEffectContextHandle);
	    if (EffectSpecHandle.IsValid())
	    {
			const FSetByCallerData& SetbyCallerData = VolumeEffectData.Value;
			EffectSpecHandle.Data->SetSetByCallerMagnitude(SetbyCallerData.SetByCallerTag, SetbyCallerData.Magnitude);

	        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			LogString += GameplayEffectClass->GetName() + TEXT(" effect applied to ") + OverlappedCharacter->GetName() + TEXT(" by ") + GetName() + TEXT("\n");
	    }

		UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);
	}
}
