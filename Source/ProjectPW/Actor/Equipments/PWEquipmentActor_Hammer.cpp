// LINK

//Default
#include "PWEquipmentActor_Hammer.h"

//Engine

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWAssetLoadManager.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

APWEquipmentActor_Hammer::APWEquipmentActor_Hammer(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APWEquipmentActor_Hammer::InitializeEquipmentActor(AActor* OwnerActor)
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

	Montage_HammerSmash = UPWAnimDataAsset::GetAnimMontage(this, CharacterData->CharacterType, EAnimMontageType::HammerSmash);
	Montage_Build = UPWAnimDataAsset::GetAnimMontage(this, CharacterData->CharacterType, EAnimMontageType::Build);
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
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);
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

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(PWPlayerCharacter->GetController());
	if(IsValid(PWPlayerController) == false)
	{
		ensure(false);
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
		PWPlayerController->CS_SpawnActor(BuildObjectClass, HitResult.Location);
	}
}
