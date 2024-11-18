// LINK

//Default
#include "PWAttributeSet_Damageable.h"

//Engine

//Game
#include "Core/PWEventManager.h"

void UPWAttributeSet_Damageable::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	if (Attribute == GetHealthAttribute())
	{
		EventManager->HealthChangedDelegate.Broadcast(GetOwningActor(), GetMaxHealth(), NewValue);
	}
}
