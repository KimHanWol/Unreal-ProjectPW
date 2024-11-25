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

void UPWAttributeSet_Damageable::BindAttributeChanged()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute()).AddUObject(this, &UPWAttributeSet_Damageable::OnHealthValueChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).AddUObject(this, &UPWAttributeSet_Damageable::OnHealthValueChanged);
	}
}

void UPWAttributeSet_Damageable::UnbindAttributeChanged()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).RemoveAll(this);
	}
}

void UPWAttributeSet_Damageable::OnRep_Health()
{
	//Client
	FOnAttributeChangeData OnAttributeChangeData;
	OnAttributeChangeData.OldValue = GetHealth();
	OnAttributeChangeData.NewValue = GetHealth();
	OnAttributeChangeData.Attribute = GetHealthAttribute();

	OnHealthValueChanged(OnAttributeChangeData);
}

void UPWAttributeSet_Damageable::OnRep_MaxHealth()
{
	//Client
	FOnAttributeChangeData OnAttributeChangeData;
	OnAttributeChangeData.OldValue = GetMaxHealth();
	OnAttributeChangeData.NewValue = GetMaxHealth();
	OnAttributeChangeData.Attribute = GetMaxHealthAttribute();

	OnHealthValueChanged(OnAttributeChangeData);
}

void UPWAttributeSet_Damageable::OnHealthValueChanged(const FOnAttributeChangeData& ChangeData)
{
	const UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->HealthChangedDelegate.Broadcast(GetOwningActor(), GetMaxHealth(), GetHealth());
	}
}

