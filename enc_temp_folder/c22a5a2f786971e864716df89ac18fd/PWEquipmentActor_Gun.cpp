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
#include "UObject/ScriptInterface.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"

APWEquipmentActor_Gun::APWEquipmentActor_Gun(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

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

void APWEquipmentActor_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EqiupmentActorLineTrace(HitResult, ViewPointRotation);

	if (bHitSuccess != bIsTargetOn)
	{
		bIsTargetOn = bHitSuccess;

		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TargetIsOnCrosshairDelegate.Broadcast(bIsTargetOn);
		}
	}
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
	if (MuzzleEffect.IsNull() == false)
	{
		UPWGameplayStatics::SpawnEmitterAttached(
			MuzzleEffect.LoadSynchronous(),
			GetMesh(),
			TEXT("MuzzleFlashSocket")
		);
	}

	EnableADS(true);

	FHitResult HitResult;
	FRotator ViewPointRotation;
	//Hit Test
	bool bHitSuccess = EqiupmentActorLineTrace(HitResult, ViewPointRotation);

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

		UPWAttributeSet_Attackable* OwnerAttribute_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
		if (IsValid(OwnerAttribute_Attackable) == true)
		{
			OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), OwnerAttribute_Attackable->GetDamage());
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

bool APWEquipmentActor_Gun::EqiupmentActorLineTrace(FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	bool bHitSuccess = false;
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return bHitSuccess;
	}

	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		return bHitSuccess;
	}

	FVector ViewPointLocation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, OutViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + OutViewPointRotation.Vector() * MaxRange;

	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
	}

	return bHitSuccess;
}
