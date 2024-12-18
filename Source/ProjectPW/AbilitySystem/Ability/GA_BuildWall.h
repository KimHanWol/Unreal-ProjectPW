// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"

//Default
#include "GA_BuildWall.generated.h"


UCLASS()
class PROJECTPW_API UGA_BuildWall : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BuildObjectClass;

	UPROPERTY(Transient)
	class UAnimMontage* BuildAnimMontage;
};
