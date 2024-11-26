// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "PWDamageableInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UPWDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPWDamageableInterface
{
	GENERATED_BODY()

public:
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const = 0;
	virtual class UPWAttributeSet_Damageable* GetPWAttributeSet_Damageable() const = 0;

	virtual void ApplyDamage(class IPWAttackableInterface* Attacker, float DamageAmount);
	virtual void ApplyHeal(class IPWHealableInterface* Healer, float HealAmount);

	virtual void PostApplyDamage(class IPWAttackableInterface* Attacker);

private:

	virtual void OnFullyDamaged(class IPWAttackableInterface* Killer) = 0;
};
