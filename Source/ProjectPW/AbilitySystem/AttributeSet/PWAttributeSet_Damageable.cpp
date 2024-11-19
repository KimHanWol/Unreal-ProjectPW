// LINK

//Default
#include "PWAttributeSet_Damageable.h"

//Engine
#include "Net/UnrealNetwork.h"

//Game
#include "Core/PWEventManager.h"

void UPWAttributeSet_Damageable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_Damageable, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPWAttributeSet_Damageable, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPWAttributeSet_Damageable::OnRep_Health()
{
	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	EventManager->HealthChangedDelegate.Broadcast(GetOwningActor(), GetMaxHealth(), GetHealth());
}

void UPWAttributeSet_Damageable::OnRep_MaxHealth()
{
	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	EventManager->HealthChangedDelegate.Broadcast(GetOwningActor(), GetMaxHealth(), GetHealth());
}
