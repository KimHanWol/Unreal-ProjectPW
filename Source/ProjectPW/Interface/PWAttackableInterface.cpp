// LINK

//Default
#include "PWAttackableInterface.h"

//Engine

//Game
#include "PWDamageableInterface.h"

void IPWAttackableInterface::GiveDamage(IPWDamageableInterface* Victim, float DamageAmount)
{
	Victim->ApplyDamage(this, DamageAmount);
}
