// LINK

//Default
#include "PWEquipmentActor_Hammer.h"

//Engine

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"

APWEquipmentActor_Hammer::APWEquipmentActor_Hammer(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APWEquipmentActor_Hammer::BeginPlay()
{	
	Super::BeginPlay();

	Montage_HammerSmash = UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::HammerSmash);
	Montage_Build = UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::Build);
	UPWAssetLoadManager::AsyncLoad(this, Montage_HammerSmash, Montage_Build);

	BuildPreviewObject = GetWorld()->SpawnActor(BuildObjectClass);
	BuildPreviewObject->SetActorEnableCollision(false);
	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(BuildPreviewObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (IsValid(StaticMeshComponent) == true)
	{
		StaticMeshComponent->SetVisibility(false);
		if (IsValid(BuildPreviewMI) == true)
		{
			for (int32 i = 0; i < StaticMeshComponent->GetMaterials().Num(); i++)
			{
				StaticMeshComponent->SetMaterial(i, BuildPreviewMI);
			}
		}
	}
}

void APWEquipmentActor_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBuildPreviewMode == false)
	{
		return;
	}

	if (IsValid(BuildPreviewObject) == false)
	{
		return;
	}

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(BuildPreviewObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (IsValid(StaticMeshComponent) == false)
	{
		return;
	}

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(BuildDistance, ECollisionChannel::ECC_WorldStatic, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		BuildPreviewObject->SetActorLocation(HitResult.Location);
		StaticMeshComponent->SetVisibility(true);
	}
	else
	{
		StaticMeshComponent->SetVisibility(false);
	}
}

bool APWEquipmentActor_Hammer::IsInteractableActor(AActor* TargetActor)
{
	IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(GetOwner());
	IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(TargetActor);
	if (AttackableSource == nullptr || DamageableTarget == nullptr)
	{
		return false;
	}

	return true;
}

void APWEquipmentActor_Hammer::Execute_Main_Triggered()
{
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_EngineTraceChannel1, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
		if (IsValid(OwnerCharacter) == false)
		{
			ensure(false);
			return;
		}

		const FVector& ShotDirection = -ViewPointRotation.Vector();

		if (IsInteractableActor(HitResult.GetActor()) == true)
		{
			UPWAttributeSet_Attackable* OwnerAttribute_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
			if (IsValid(OwnerAttribute_Attackable) == true)
			{
				OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), OwnerAttribute_Attackable->GetDamage());
			}
		}
	}
	
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == true)
	{
		PWPlayerCharacter->PlayMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, Montage_HammerSmash));
	}
}

void APWEquipmentActor_Hammer::Execute_Sub_Triggered()
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false || PWPlayerCharacter->IsLocallyControlled() == false)
	{
		return;
	}

	bBuildPreviewMode = true;
}

void APWEquipmentActor_Hammer::Execute_Sub_Completed()
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false || PWPlayerCharacter->IsLocallyControlled() == false)
	{
		return;
	}

	bBuildPreviewMode = false;

	if (IsValid(BuildPreviewObject) == true)
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(BuildPreviewObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		{
			StaticMeshComponent->SetVisibility(false);
		}
	}

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(BuildDistance, ECollisionChannel::ECC_WorldStatic, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		PWPlayerCharacter->PlayMontage(UPWAssetLoadManager::GetLoadedAsset<UAnimMontage>(this, Montage_Build));

		AActor* BuildObject = GetWorld()->SpawnActor<AActor>(BuildObjectClass);
		BuildObject->SetActorLocation(HitResult.Location);
		BuildObject->SetActorEnableCollision(true);
	}
}
