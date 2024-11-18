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
		
	AActor* Instigator = InstigatorASC->GetOwner();

	FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
	ContextHandle.AddInstigator(Instigator, Instigator);

	UClass* DamageEffectClass = UPWGameData::GetGameplayEffect(Instigator, EGameplayEffectType::AddHealth)->GetClass();
	if (IsValid(DamageEffectClass) == false)
	{
		ensure(false);
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		DamageEffectClass,
		1.f,
		ContextHandle
	);

	UE_LOG(LogTemp, Warning, TEXT("%s Damaged \nPrev Health : %f"), *Instigator->GetName(), Cast<UPWAttributeSet_Damageable>(InstigatorASC->GetAttributeSet(UPWAttributeSet_Damageable::StaticClass()))->GetHealth());
	if (SpecHandle.Data.IsValid() == true)
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(TEXT("SetByCaller.Common.Amount"))), -DamageAmount);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
	UE_LOG(LogTemp, Warning, TEXT("%s Damaged \nCurrent Health : %f"), *Instigator->GetName(), Cast<UPWAttributeSet_Damageable>(InstigatorASC->GetAttributeSet(UPWAttributeSet_Damageable::StaticClass()))->GetHealth());

	PostApplyDamage();
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
		
	AActor* Instigator = InstigatorASC->GetOwner();

	FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
	ContextHandle.AddInstigator(Instigator, Instigator);

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		UPWGameData::GetGameplayEffect(Instigator, EGameplayEffectType::AddHealth)->GetClass(),
		1.f,
		ContextHandle
	);

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(TEXT("SetByCaller.Common.Amount"))), HealAmount);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void IPWDamageableInterface::PostApplyDamage()
{
	const UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponent();
	if (IsValid(OwnerASC) == false)
	{
		ensure(false);
		return;
	}

	const UPWAttributeSet_Damageable* OwnerAttributeSet_Damageable = Cast<UPWAttributeSet_Damageable>(OwnerASC->GetAttributeSet(UPWAttributeSet_Damageable::StaticClass()));
	if (IsValid(OwnerAttributeSet_Damageable) == false)
	{
		ensure(false);
		return;
	}

	float OwnerHealth = OwnerAttributeSet_Damageable->GetHealth();
	if (OwnerHealth <= 0)
	{
		OnFullyDamaged();
	}
}

void IPWDamageableInterface::OnFullyDamaged()
{
	//TODO: 총 남은 캐릭터 수 변경 이벤트 보내기
}
