// LINK

//Default
#include "PWEquipmentActor_Bandage.h"

//Engine
#include "UObject/ScriptInterface.h"


//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Interface/PWDamageableInterface.h"
#include "Interface/PWHealableInterface.h"

void APWEquipmentActor_Bandage::InitializeEquipmentActor(AActor* OwnerActor)
{
	Super::InitializeEquipmentActor(OwnerActor);

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(OwnerActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	const FPWCharacterDataTableRow* CharacterData = OwnerCharacter->GetCharacterData(); 
	if (CharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	Montage_Bandage = UPWAnimDataAsset::GetAnimMontage(this, CharacterData->CharacterType, EAnimMontageType::Bandage);
	UPWAssetLoadManager::AsyncLoad(this, Montage_Bandage);
}

void APWEquipmentActor_Bandage::Execute_Main_Triggered()
{
	Super::Execute_Main_Triggered();

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	OwnerCharacter->PlayMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, Montage_Bandage));

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

bool APWEquipmentActor_Bandage::IsInteractableActor(AActor* TargetActor)
{
	IPWHealableInterface* HealableSource = Cast<IPWHealableInterface>(GetOwner());
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (HealableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}
