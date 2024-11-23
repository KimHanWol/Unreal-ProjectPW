// LINK

//Default
#include "PWEquipmentActor_Bandage.h"

//Engine
#include "AbilitySystemComponent.h"
#include "UObject/ScriptInterface.h"


//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWDamageableInterface.h"
#include "Interface/PWHealableInterface.h"

void APWEquipmentActor_Bandage::Execute_Main_Triggered()
{
	Super::Execute_Main_Triggered();

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	TSoftObjectPtr<UAnimMontage> Montage_Bandage = UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::Bandage);
	if (Montage_Bandage.IsNull() == false)
	{
		OwnerCharacter->PlayMontage(Montage_Bandage.LoadSynchronous());
	}

	//Hit Test
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(HitResult, ViewPointRotation);

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
