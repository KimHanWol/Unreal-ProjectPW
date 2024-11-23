// LINK

//Default
#include "PWEquipmentActor_Gun.h"

//Engine
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ScriptInterface.h" 

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"

//TArray<FSoftObjectPath> ItemsToStream;
//if (MuzzleEffect.IsNull() == false && ImpactEffect.IsNull() == false && Montage_ADS.IsNull() == false)
//{
//	ItemsToStream.AddUnique(MuzzleEffect.ToSoftObjectPath());
//	ItemsToStream.AddUnique(ImpactEffect.ToSoftObjectPath());
//	ItemsToStream.AddUnique(Montage_ADS.ToSoftObjectPath());
//}

// TODO: void AddSoftObjectReferences(std::initializer_list<TSoftObjectPtr<AActor>> References);
// 가변인자 받을 수 있게 수정해서 더 간편하게 호출하기
// AssetLoadManager 같은거 만들어서 SoftObjectPath로 Map 만들고 캐시한 데이터 가져올 수 있게 해보자.
//UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);

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
	if (MuzzleEffect.IsNull() == false)
	{
		UPWGameplayStatics::SpawnEmitterAttached(
			MuzzleEffect.LoadSynchronous(),
			GetMesh(),
			TEXT("MuzzleFlashSocket")
		);
	}

	EnableADS(true);

	//Hit Test
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(HitResult, ViewPointRotation);

	//Hit Success
	if (bHitSuccess == true)
	{
		const FVector& ShotDirection = -ViewPointRotation.Vector();
		if (ImpactEffect.IsNull() == false)
		{
			UPWGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect.LoadSynchronous() ,HitResult.Location, ShotDirection.Rotation());
		}

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

	TSoftObjectPtr<UAnimMontage> Montage_ADS = UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::ADS);
	if (Montage_ADS.IsNull() == false)
	{
		if (bEnabled == true)
		{
			OwnerCharacter->PlayMontage(Montage_ADS);
			bIsADSAnimPlaying = true;
		}
		else
		{
			OwnerCharacter->StopMontage(Montage_ADS);
			bIsADSAnimPlaying = false;
		}
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
