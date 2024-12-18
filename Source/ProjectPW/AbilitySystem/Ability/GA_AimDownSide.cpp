// LINK

//Default
#include "GA_AimDownSide.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"




void UGA_AimDownSide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	TSoftObjectPtr<UAnimMontage> SoftMontageADS = OwnerAnimDataAsset->GetAnimMontage(this, OwnerCharacterData->CharacterType, EAnimMontageType::ADS);
	if (SoftMontageADS.IsNull() == false)
	{
		MontageADS = UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, SoftMontageADS);
	}

	if (ensure(IsValid(MontageADS) == true))
	{
		OwnerCharacter->PlayMontage(MontageADS);
	}
}

void UGA_AimDownSide::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

	if (ensure(IsValid(MontageADS) == true))
	{
		OwnerCharacter->StopMontage(MontageADS);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
