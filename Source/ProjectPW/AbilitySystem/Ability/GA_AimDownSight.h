﻿// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"

//Default
#include "GA_AimDownSight.generated.h"


UCLASS()
class PROJECTPW_API UGA_AimDownSight : public UPWGameplayAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UPROPERTY(Transient)
	UAnimMontage* MontageADS;

	UPROPERTY(Transient)
	float DefaultCameraFOV;

	UPROPERTY(EditAnywhere)
	float AimCameraFOV;

	UPROPERTY(EditAnywhere)
	float AimCameraFOVDuration = 0.3f;

	UPROPERTY(Transient)
	float CurrentTime = 0.f;

	FTimerHandle ADSTimerHandle;
	FTimerHandle ADSCancelTimerHandle;
};
