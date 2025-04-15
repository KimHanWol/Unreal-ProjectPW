// LINK

//Default
#include "GA_AimDownSight.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"



void UGA_AimDownSight::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	APlayerCameraManager* PlayerCameraManager = UPWGameplayStatics::GetPlayerCameraManager(this, 0);
	if (IsValid(PlayerCameraManager) == false)
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

	if(OwnerCharacter->IsLocallyControlled() == true)
	{
		DefaultCameraFOV = PlayerCameraManager->GetFOVAngle();

		GetWorld()->GetTimerManager().SetTimer(ADSTimerHandle, [this, PlayerCameraManager]() {
			PlayerCameraManager->SetFOV(FMath::Lerp(DefaultCameraFOV, AimCameraFOV, CurrentTime / AimCameraFOVDuration));
			if (CurrentTime >= AimCameraFOVDuration)
			{
				GetWorld()->GetTimerManager().ClearTimer(ADSTimerHandle);
				return;
			}
			CurrentTime += GetWorld()->GetDeltaSeconds();
			}, 0.01f, true);
	}
}

void UGA_AimDownSight::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

	APlayerCameraManager* PlayerCameraManager = UPWGameplayStatics::GetPlayerCameraManager(this, 0);
	if (IsValid(PlayerCameraManager) == false)
	{
		ensure(false);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	if (ensure(IsValid(MontageADS) == true))
	{
		OwnerCharacter->StopMontage(MontageADS);
	}

	PlayGameplayCue();

	if (OwnerCharacter->IsLocallyControlled() == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(ADSTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ADSCancelTimerHandle, [this, PlayerCameraManager]() {
			PlayerCameraManager->SetFOV(FMath::Lerp(DefaultCameraFOV, AimCameraFOV, CurrentTime / AimCameraFOVDuration));
			CurrentTime -= GetWorld()->GetDeltaSeconds();
			if (CurrentTime <= 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(ADSCancelTimerHandle);
				return;
			}
			CurrentTime -= GetWorld()->GetDeltaSeconds();
			}, 0.01f, true);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
