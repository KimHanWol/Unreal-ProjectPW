// LINK

//Default
#include "GA_Fire.h"

//Engine
#include "AbilitySystemComponent.h"
#include "Particles/ParticleSystem.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Equipments/PWEquipmentActorBase.h"
#include "Component/PWEquipmentComponent.h"
#include "Core/PWAssetLoadManager.h"
#include "Helper/PWGameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	UAbilitySystemComponent* OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (IsValid(OwnerASC) == false)
	{
		ensure(false);
		return;
	}

	UPWEquipmentComponent* OwnerEquipmentComponent = OwnerCharacter->GetComponentByClass<UPWEquipmentComponent>();
	if (IsValid(OwnerEquipmentComponent) == false)
	{
		ensure(false);
		return;
	}

	APWEquipmentActorBase* OwnerEquipmentActor = OwnerEquipmentComponent->GetSpawnedEquipmentActor();
	if (IsValid(OwnerEquipmentActor) == false)
	{
		ensure(false);
		return;
	}

	//ADS 적용
	if (ensure(IsValid(AimDownSightClass) == true))
	{
		OwnerASC->TryActivateAbilityByClass(AimDownSightClass);
	}
	
	//TODO: EffectDataTable 만들어서 Asyncload 하기

	TArray<UActorComponent*> MuzzlePointComponentList =	OwnerEquipmentActor->GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("MuzzlePoint"));
	if (MuzzlePointComponentList.Num() > 0)
	{
		USceneComponent* MuzzlePointSeceneComponent = Cast<USceneComponent>(MuzzlePointComponentList[0]);
		if (IsValid(MuzzlePointSeceneComponent) == true)
		{
			//Muzzle Effect
			//MuzzleEffectEmitter = UPWGameplayStatics::SpawnEmitterAttached(
			//	UPWAssetLoadManager::GetLoadedAsset<UParticleSystem>(this, MuzzleEffect),
			//	MuzzlePointSeceneComponent
			//);

			MuzzleEffectEmitter = UPWGameplayStatics::SpawnEmitterAtLocation(
				this, 
				UPWAssetLoadManager::GetLoadedAsset<UParticleSystem>(this, MuzzleEffect),
				MuzzlePointSeceneComponent->GetComponentLocation(), 
				MuzzlePointSeceneComponent->GetComponentRotation()
			);
		}
	}

	//Hit Test
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);

	//Hit Success
	if (bHitSuccess == true)
	{
		const FVector& ShotDirection = -ViewPointRotation.Vector();
		UPWGameplayStatics::SpawnEmitterAtLocation(
			this, 
			UPWAssetLoadManager::GetLoadedAsset<UParticleSystem>(this, ImpactEffect),
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

void UGA_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

	UAbilitySystemComponent* OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (IsValid(OwnerASC) == false)
	{
		ensure(false);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	UPWEquipmentComponent* OwnerEquipmentComponent = OwnerCharacter->GetComponentByClass<UPWEquipmentComponent>();
	if (IsValid(OwnerEquipmentComponent) == false)
	{
		ensure(false);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	//TODO: 종속성 수정
	if (OwnerEquipmentComponent->IsSubTriggered() == false)
	{
		if (ensure(IsValid(AimDownSightClass) == true))
		{
			OwnerASC->CancelAbility(AimDownSightClass.GetDefaultObject());
		}
	}

	if (IsValid(MuzzleEffectEmitter) == true)
	{
		MuzzleEffectEmitter->DestroyComponent();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_Fire::IsInteractableActor(AActor* TargetActor)
{
	if (GetCurrentActorInfo() == nullptr)
	{
		ensure(false);
		return false;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetCurrentActorInfo()->AvatarActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return false;
	}

	IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(OwnerCharacter);
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (AttackableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}
