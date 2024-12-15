// LINK

//Default
#include "PWEquipmentComponent.h"

//Engine

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"


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

void UPWEquipmentComponent::OnAliveStateChanged(bool bAlive)
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		ensure(false);
		return;
	}

	SpawnedEquipmentActor->SetActorHiddenInGame(bAlive);
}

void UPWEquipmentComponent::SpawnEquipmentActor(ECharacterType OwnerCharacterType)
{
	const FPWCharacterDataTableRow* CharacterData = UPWGameplayStatics::FindCharacterData(this, OwnerCharacterType);
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
		SpawnedEquipmentActor->InitializeEquipmentActor(GetOwner());
	}
}
