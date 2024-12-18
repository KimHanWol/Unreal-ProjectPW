// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"

//Default
#include "GA_BuildWall_Preview.generated.h"


UCLASS()
class PROJECTPW_API UGA_BuildWall_Preview : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnTick(class UPWAbilityTask_Tick* AbilityTask_Tick, float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BuildPreivewObjectClass;

	UPROPERTY(Transient)
	AActor* BuildPreviewObject;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInstance* BuildPreviewMI;

	UPROPERTY(Transient)
	bool bBuildPreviewMode = false;

};
