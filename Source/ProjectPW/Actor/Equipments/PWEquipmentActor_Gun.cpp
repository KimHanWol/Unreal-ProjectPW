// LINK

//Default
#include "PWEquipmentActor_Gun.h"

//Engine
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"

void APWEquipmentActor_Gun::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSoftObjectPath> ItemsToStream;
	if (MuzzleEffect.IsNull() == false && ImpactEffect.IsNull() == false && Montage_ADS.IsNull() == false)
	{
		ItemsToStream.AddUnique(MuzzleEffect.ToSoftObjectPath());
		ItemsToStream.AddUnique(ImpactEffect.ToSoftObjectPath());
		ItemsToStream.AddUnique(Montage_ADS.ToSoftObjectPath());
	}

	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);
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

	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
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
	bool bHitSuccess = false;
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FHitResult HitResult;
	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
	}

	if (bHitSuccess == true)
	{
		//Hit Success
		const FVector& ShotDirection = -ViewPointRotation.Vector();
		if (ImpactEffect.IsNull() == false)
		{
			UPWGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect.LoadSynchronous() ,HitResult.Location, ShotDirection.Rotation());
		}

		IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(GetOwner());
		IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(HitResult.GetActor());
		if (AttackableSource == nullptr || DamageableTarget == nullptr)
		{
			return;
		}

		//Apply Damage
		const UPWAttributeSet_Attackable* OwnerAttributeSet_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
		if (IsValid(OwnerAttributeSet_Attackable) == true)
		{
			DamageableTarget->ApplyDamage(AttackableSource, OwnerAttributeSet_Attackable->GetDamage());
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

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (Montage_ADS.IsNull() == false)
	{
		if (bEnabled == true)
		{
			OwnerCharacter->PlayAnimMontage(Montage_ADS.LoadSynchronous());
			bIsADSAnimPlaying = true;
		}
		else
		{
			OwnerCharacter->StopAnimMontage(Montage_ADS.LoadSynchronous());
			bIsADSAnimPlaying = false;
		}
	}
}
