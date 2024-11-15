// LINK

//Default
#include "PWEquipmentComponent.h"

//Engine
#include "GameFramework/Character.h"

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"


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

void UPWEquipmentComponent::Execute_Main()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Main();
}

void UPWEquipmentComponent::Execute_Sub()
{
	if (IsValid(SpawnedEquipmentActor) == false)
	{
		return;
	}

	SpawnedEquipmentActor->Execute_Sub();
}

void UPWEquipmentComponent::SpawnEquipmentActor()
{
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
