// LINK

//Default
#include "PWAttributeSet_Attackable.h"

//Engine
#include "Net/UnrealNetwork.h"

//Game

void UPWAttributeSet_Attackable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPWAttributeSet_Attackable, Damage);
}