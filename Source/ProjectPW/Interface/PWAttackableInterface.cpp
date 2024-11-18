// LINK

//Default
#include "PWAttackableInterface.h"
#include "PWDamageableInterface.h"

//Engine

//Game

void IPWAttackableInterface::GiveDamage(IPWDamageableInterface* Victim, float DamageAmount)
{
	Victim->ApplyDamage(this, DamageAmount);
}
