// LINK

//Default
#include "PWEquipmentActor_Gun.h"

//Engine
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ScriptInterface.h" 

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"

void APWEquipmentActor_Gun::BeginPlay()
{
	Super::BeginPlay();

	Montage_ADS = UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::ADS);
	UPWAssetLoadManager::AsyncLoad(this, MuzzleEffect, ImpactEffect, Montage_ADS);
}

void APWEquipmentActor_Gun::Execute_Main_Triggered()
{
	Super::Execute_Main_Triggered();

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	//Muzzle Effect
	UPWGameplayStatics::SpawnEmitterAttached(
		UPWAssetLoadManager::GetLoadedAsset<UParticleSystem>(this, MuzzleEffect),
		GetMesh(),
		TEXT("MuzzleFlashSocket")
	);

	EnableADS(true);

	//Hit Test
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(HitResult, ViewPointRotation);

	//Hit Success
	if (bHitSuccess == true)
	{
		const FVector& ShotDirection = -ViewPointRotation.Vector();
		UPWGameplayStatics::SpawnEmitterAtLocation(
			this, 
			UPWAssetLoadManager::GetLoadedAsset<UParticleSystem>(this, MuzzleEffect),
			HitResult.Location, 
			ShotDirection.Rotation()
		);

		if (IsInteractableActor(HitResult.GetActor()) == true)
		{
			UPWAttributeSet_Attackable* OwnerAttribute_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
			if (IsValid(OwnerAttribute_Attackable) == true)
			{
				OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), OwnerAttribute_Attackable->GetDamage());
			}
		}
	}
}

void APWEquipmentActor_Gun::Execute_Main_Completed()
{
	if (bIsSubExecuting == false)
	{
		EnableADS(false);
	}
}

void APWEquipmentActor_Gun::Execute_Sub_Triggered()
{
	Super::Execute_Sub_Triggered();

	bIsSubExecuting = true;
	EnableADS(true);
}

void APWEquipmentActor_Gun::Execute_Sub_Completed()
{
	Super::Execute_Sub_Completed();

	bIsSubExecuting = false;
	EnableADS(false);
}

void APWEquipmentActor_Gun::EnableADS(bool bEnabled)
{
	if (bEnabled == bIsADSAnimPlaying)
	{
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (bEnabled == true)
	{
		OwnerCharacter->PlayMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, Montage_ADS));
		bIsADSAnimPlaying = true;
	}
	else
	{
		OwnerCharacter->StopMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, Montage_ADS));
		bIsADSAnimPlaying = false;
	}
}

bool APWEquipmentActor_Gun::IsInteractableActor(AActor* TargetActor)
{
	IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(GetOwner());
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (AttackableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}
