// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"

//Default
#include "GA_Fire.generated.h"


UCLASS()
class PROJECTPW_API UGA_Fire : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool IsInteractableActor(AActor* TargetActor) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPWGameplayAbilityBase> AimDownSightClass;

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzleEffectEmitter;
};
