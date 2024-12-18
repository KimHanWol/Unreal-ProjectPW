// LINK

//Default
#include "GA_Bandage.h"

//Engine

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Interface/PWDamageableInterface.h"
#include "Interface/PWHealableInterface.h"

void UGA_Bandage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo == nullptr)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(ActorInfo->AvatarActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	const FPWCharacterDataTableRow* OwnerCharacterData = OwnerCharacter->GetCharacterData();
	if (OwnerCharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	UPWAnimDataAsset* OwnerAnimDataAsset = OwnerCharacterData->AnimDataAsset;
	if (IsValid(OwnerAnimDataAsset) == false)
	{
		ensure(false);
		return;
	}

 	TSoftObjectPtr<UAnimMontage> SoftMontage_Bandage = OwnerAnimDataAsset->GetAnimMontage(this, OwnerCharacterData->CharacterType, EAnimMontageType::Bandage);
	if (SoftMontage_Bandage.IsNull() == false)
	{
		Montage_Bandage = UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, SoftMontage_Bandage);
	}

	if (ensure(IsValid(Montage_Bandage) == true))
	{
		OwnerCharacter->PlayMontage(Montage_Bandage);
	}

	//Hit Test
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);

	//Hit Success
	if (bHitSuccess == true)
	{
		if (IsInteractableActor(HitResult.GetActor()) == true)
		{
			const UPWAttributeSet_Healable* OwnerAttributeSet_Healable = OwnerCharacter->GetPWAttributeSet_Healable();
			if (IsValid(OwnerAttributeSet_Healable) == true)
			{
				OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), -OwnerAttributeSet_Healable->GetHealAmount());
			}
		}
	}
}

void UGA_Bandage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo == nullptr)
	{
		ensure(false);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(ActorInfo->AvatarActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	//if (ensure(IsValid(Montage_Bandage) == true))
	//{
	//	OwnerCharacter->StopMontage(Montage_Bandage);
	//}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_Bandage::IsInteractableActor(AActor* TargetActor)
{
	if (GetCurrentActorInfo() == nullptr)
	{
		ensure(false);
		return false;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetCurrentActorInfo()->AvatarActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return false;
	}

	IPWHealableInterface* HealableSource = Cast<IPWHealableInterface>(OwnerCharacter);
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (HealableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}
