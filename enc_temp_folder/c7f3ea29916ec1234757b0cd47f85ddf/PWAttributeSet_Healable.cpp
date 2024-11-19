// LINK

//Default
#include "PWAttributeSet_Healable.h"

//Engine
#include "Net/UnrealNetwork.h"

//Game

void UPWAttributeSet_Healable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPWAttributeSet_Healable, HealAmount);
}

void UPWAttributeSet_Healable::OnRep_HealAmount()
{
}
