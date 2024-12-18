// LINK

//Default
#include "PWAbilityTask_Tick.h"

//Engine

//Game


UPWAbilityTask_Tick::UPWAbilityTask_Tick(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	TimeLimits = 0.f;
	TimeElapsed = 0.f;
}

void UPWAbilityTask_Tick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	TimeElapsed += DeltaTime;
	OnTick.ExecuteIfBound(this, DeltaTime);

	if (TimeLimits >= 0.0f && TimeElapsed >= TimeLimits)
	{
		OnTimeOver.ExecuteIfBound(this);
		EndTask();
	}
}

UPWAbilityTask_Tick* UPWAbilityTask_Tick::MakeTickTask(UGameplayAbility* OwningAbility, float TimeLimits)
{
	UPWAbilityTask_Tick* MyObj = NewAbilityTask<UPWAbilityTask_Tick>(OwningAbility);

	MyObj->TimeLimits = TimeLimits;
	MyObj->TimeElapsed = 0.0f;

	return MyObj;
}