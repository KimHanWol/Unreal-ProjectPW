// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "PWAttackableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPWAttackableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPWAttackableInterface
{
	GENERATED_BODY()

public:

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const = 0;
	virtual class UPWAttributeSet_Attackable* GetPWAttributeSet_Attackable() const = 0;

	virtual void GiveDamage(class IPWDamageableInterface* Victim, float DamageAmount);
};
