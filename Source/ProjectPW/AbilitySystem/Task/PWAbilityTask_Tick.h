// LINK

#pragma once

//Engine
#include "Abilities/Tasks/AbilityTask.h"
#include "CoreMinimal.h"

//Game

//Default
#include "PWAbilityTask_Tick.generated.h"


UCLASS()
class PROJECTPW_API UPWAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

protected:

	UPWAbilityTask_Tick(const FObjectInitializer& ObjectInitializer);

	virtual void TickTask(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks")
	static UPWAbilityTask_Tick* MakeTickTask(UGameplayAbility* OwningAbility, float TimeLimits);

public:

	DECLARE_DELEGATE_TwoParams(FTickEvent, UPWAbilityTask_Tick* AbilityTask_Tick, float DeltaTime);
	FTickEvent OnTick;

	DECLARE_DELEGATE_OneParam(FTimeOverEvent, UPWAbilityTask_Tick* AbilityTask_Tick);
	FTimeOverEvent OnTimeOver;

protected:

	UPROPERTY(Transient)
	float TimeLimits = 0.f;

	UPROPERTY(Transient)
	float TimeElapsed = 0.f;
};
