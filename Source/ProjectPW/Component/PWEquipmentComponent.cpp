// LINK

//Default
#include "PWEquipmentComponent.h"

//Engine

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"


void UPWEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	//Hide already spawned useless bone
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == true)
	{
		USkeletalMeshComponent* SkeletalMesh = OwnerCharacter->GetMesh();
		if (IsValid(SkeletalMesh) == true)
		{
			OwnerSkeletalMesh = SkeletalMesh;
			OwnerSkeletalMesh->HideBoneByName(TEXT("weapon_r"), PBO_None);
		}
	}

	SpawnEquipmentActor();
}

void UPWEquipmentComponent::Execute_Main_Triggered()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Main_Triggered();
}

void UPWEquipmentComponent::Execute_Main_Completed()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Main_Completed();
}

void UPWEquipmentComponent::Execute_Sub_Triggered()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Sub_Triggered();
}

void UPWEquipmentComponent::Execute_Sub_Completed()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Sub_Completed();
}

void UPWEquipmentComponent::OnDeath()
{
	SpawnedEquipmentActor->Destroy();
	SpawnedEquipmentActor = nullptr;
}

void UPWEquipmentComponent::SpawnEquipmentActor()
{
	const APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}
		
	const FPWCharacterDataTableRow* CharacterData = OwnerCharacter->GetCharacterData();
	if (CharacterData == nullptr)
	{
		return;
	}

	TSubclassOf<class APWEquipmentActorBase> EquipmentActorClass = CharacterData->EquipmentActorClass;
	if (IsValid(EquipmentActorClass) == false)
	{
		return;
	}

	if (IsValid(GetWorld()) == false)
	{
		return;
	}

	SpawnedEquipmentActor = GetWorld()->SpawnActor<APWEquipmentActorBase>(EquipmentActorClass);
	if (IsValid(OwnerSkeletalMesh) == true)
	{
		SpawnedEquipmentActor->AttachToComponent(OwnerSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		SpawnedEquipmentActor->SetOwner(GetOwner());
	}
}
