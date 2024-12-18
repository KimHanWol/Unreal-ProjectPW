// LINK

//Default
#include "GA_BuildWall.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

void UGA_BuildWall::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo == nullptr)
	{
		ensure(false);
		return;
	}

	//TODO: Local??
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(ActorInfo->AvatarActor);
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	const FPWCharacterDataTableRow* CharacterData = PWPlayerCharacter->GetCharacterData();
	if (CharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	const UPWAnimDataAsset* CharacterAnimDataAsset = CharacterData->AnimDataAsset;
	if (IsValid(CharacterAnimDataAsset) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(PWPlayerCharacter->GetController());
	if(IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_WorldStatic, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		TSoftObjectPtr<UAnimMontage> SoftBuildAnimMontage = CharacterAnimDataAsset->GetAnimMontage(this, CharacterData->CharacterType, EAnimMontageType::Build);
		if (ensure(SoftBuildAnimMontage.IsNull() == false))
		{
			BuildAnimMontage = UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, SoftBuildAnimMontage);
		}

		if (ensure(IsValid(BuildAnimMontage) == true))
		{
			PWPlayerCharacter->PlayMontage(BuildAnimMontage);
		}

		FVector LookVector = PWPlayerCharacter->GetActorLocation() - HitResult.Location;
		PWPlayerController->CS_SpawnActor(BuildObjectClass, HitResult.Location, LookVector.Rotation());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_BuildWall::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

	//if (ensure(IsValid(BuildAnimMontage) == true))
	//{
	//	OwnerCharacter->StopMontage(BuildAnimMontage);
	//}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
