// LINK

//Default
#include "GA_HammerSmash.h"

//Engine

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

void UGA_HammerSmash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		//Apply Damage
		if (OwnerCharacter->HasAuthority() == true)
		{
			float Damage = 0.f;
			UPWAttributeSet_Attackable* OwnerAttribute_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
			if (IsValid(OwnerAttribute_Attackable) == true)
			{
				Damage = OwnerAttribute_Attackable->GetDamage();
			}

			AActor* VictimActor = HitResult.GetActor();
			if (IsValid(VictimActor) == true && IsInteractableActor(VictimActor) == true)
			{
				IPWDamageableInterface* VictimActorInterface = Cast<IPWDamageableInterface>(VictimActor);
				IPWAttackableInterface* AttackerActorInterface = Cast<IPWAttackableInterface>(OwnerCharacter);
				if (VictimActorInterface != nullptr && AttackerActorInterface != nullptr)
				{
					VictimActorInterface->ApplyDamage(AttackerActorInterface, Damage);
				}
			}
		}
	}
	
	const FPWCharacterDataTableRow* CharacterData = OwnerCharacter->GetCharacterData();
	if (CharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	UPWAnimDataAsset* CharacterAnimDataAsset = CharacterData->AnimDataAsset;
	if (IsValid(CharacterAnimDataAsset) == false)
	{
		ensure(false);
		return;
	}

	HammerSmashAnimMontage = CharacterAnimDataAsset->GetAnimMontage(this, CharacterData->CharacterType, EAnimMontageType::HammerSmash);
	if (ensure(HammerSmashAnimMontage.IsNull() == false))
	{
		OwnerCharacter->PlayMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, HammerSmashAnimMontage));
	}
}

void UGA_HammerSmash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
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

	//if (ensure(HammerSmashAnimMontage.IsNull() == false))
	//{
	//	OwnerCharacter->StopMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, HammerSmashAnimMontage));
	//}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_HammerSmash::IsInteractableActor(AActor* TargetActor)
{
	if(Super::IsInteractableActor(TargetActor) == false)
	{
		ensure(false);
		return false;
	}

	if (GetCurrentActorInfo() == nullptr)
	{
		ensure(false);
		return false;
	}

	IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(GetCurrentActorInfo()->AvatarActor);
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (AttackableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}