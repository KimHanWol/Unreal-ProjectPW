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

	if (bMainTriggered == false)
	{
		bMainTriggered = true;
		SpawnedEquipmentActor->Execute_Main_Triggered();
	}
}

void UPWEquipmentComponent::Execute_Main_Completed()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	if (bMainTriggered == true)
	{
		bMainTriggered = false;
		SpawnedEquipmentActor->Execute_Main_Completed();
	}
}

void UPWEquipmentComponent::Execute_Sub_Triggered()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	if (bSubTriggered == false)
	{
		bSubTriggered = true;
		SpawnedEquipmentActor->Execute_Sub_Triggered();
	}
}

void UPWEquipmentComponent::Execute_Sub_Completed()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	if (bSubTriggered == true)
	{
		bSubTriggered = false;
		SpawnedEquipmentActor->Execute_Sub_Completed();
	}
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

void UPWEquipmentComponent::SpawnEquipmentActor()
{
	APWPlayerCharacter* PlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PlayerCharacter) == false)
	{
		ensure(false);
		return;
	}

	const FPWCharacterDataTableRow* CharacterData = PlayerCharacter->GetCharacterData();
	if (CharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	const FPWEquipmentDataTableRow* EquipmentData = UPWGameplayStatics::FindEquipmentData(this, CharacterData->CharacterType);
	if (EquipmentData == nullptr)
	{
		ensure(false);
		return;
	}

	if (IsValid(EquipmentData->EquipmentActor) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(GetWorld()) == false)
	{
		ensure(false);
		return;
	}

	SpawnedEquipmentActor = GetWorld()->SpawnActor<APWEquipmentActorBase>(EquipmentData->EquipmentActor);
	if (ensure(IsValid(SpawnedEquipmentActor) == true))
	{
		if (ensure(IsValid(OwnerSkeletalMesh) == true))
		{	
			SpawnedEquipmentActor->AttachToComponent(OwnerSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		}
		SpawnedEquipmentActor->InitializeEquipmentActor(GetOwner());
	}
}
