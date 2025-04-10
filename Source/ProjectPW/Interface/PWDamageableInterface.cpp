// LINK

//Default
#include "PWDamageableInterface.h"

//Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "Data/DataAsset/PWGameData.h"
#include "PWAttackableInterface.h"
#include "PWHealableInterface.h"

void IPWDamageableInterface::ApplyDamage(class IPWAttackableInterface* Attacker, float DamageAmount)
{
	if (DamageAmount == 0)
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = Attacker->GetAbilitySystemComponent();
	if (IsValid(InstigatorASC) == false)
	{
		ensure(false);
		return;
	}
		
	ApplyHealth_Internal(InstigatorASC, -DamageAmount);

	PostApplyDamage(Attacker);
}

void IPWDamageableInterface::ApplyHeal(class IPWHealableInterface* Healer, float HealAmount)
{
	if (HealAmount == 0)
	{
		return;
	}

	UAbilitySystemComponent* InstigatorASC = Healer->GetAbilitySystemComponent();
	if (IsValid(InstigatorASC) == false)
	{
		ensure(false);
		return;
	}
		
	ApplyHealth_Internal(InstigatorASC, HealAmount);
}

void IPWDamageableInterface::PostApplyDamage(IPWAttackableInterface* Attacker)
{
	const UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponent();
	if (IsValid(OwnerASC) == false)
	{
		ensure(false);
		return;
	}

	const UPWAttributeSet_Damageable* OwnerAttributeSet_Damageable = GetPWAttributeSet_Damageable();
	if (IsValid(OwnerAttributeSet_Damageable) == false)
	{
		ensure(false);
		return;
	}

	float OwnerHealth = OwnerAttributeSet_Damageable->GetHealth();
	if (OwnerHealth <= 0)
	{
		OnFullyDamaged(Attacker);
	}
}

void IPWDamageableInterface::ApplyHealth_Internal(UAbilitySystemComponent* InstigatorASC, float ChangedAmount)
{
	if (ChangedAmount == 0)
	{
		return;
	}

	if (IsValid(InstigatorASC) == false)
	{
		ensure(false);
		return;
	}
		
	AActor* Instigator = InstigatorASC->GetOwner();
	if (IsValid(Instigator) == false)
	{
		ensure(false);
		return;
	}

	if (Instigator->HasAuthority() == false)
	{
		ensure(false);
		return;
	}

	FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
	ContextHandle.AddInstigator(Instigator, Instigator);

	UClass* EffectClass;
	if (ChangedAmount < 0)
	{
		EffectClass = UPWGameData::GetGameplayEffect(Instigator, EGameplayEffectType::AddHealth).Get();
	}
	else
	{
		EffectClass = UPWGameData::GetGameplayEffect(Instigator, EGameplayEffectType::Heal).Get();
	}

	if (IsValid(EffectClass) == false)
	{
		ensure(false);
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		EffectClass,
		-1.f,
		ContextHandle
	);

	if (SpecHandle.IsValid() == true && SpecHandle.Data.IsValid() == true)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(TEXT("SetByCaller.Common.Amount"))), ChangedAmount);
		GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(EffectClass, GetAbilitySystemComponent(), true);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

		float ClampHealth = FMath::Clamp(GetPWAttributeSet_Damageable()->GetHealth(), 0.f, GetPWAttributeSet_Damageable()->GetMaxHealth());
		GetPWAttributeSet_Damageable()->SetHealth(ClampHealth);
		UE_LOG(LogTemp, Warning, TEXT("%s HealthChanged \nCurrent Health : %f"), *Instigator->GetName(), GetPWAttributeSet_Damageable()->GetHealth());
	}
}
