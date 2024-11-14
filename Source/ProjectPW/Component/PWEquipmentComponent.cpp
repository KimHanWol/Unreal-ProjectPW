// LINK

//Default
#include "PWEquipmentComponent.h"

//Engine
#include "GameFramework/Character.h"

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"

void UPWEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();

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

	APWEquipmentActorBase* PWEquipmentActorBase = GetWorld()->SpawnActor<APWEquipmentActorBase>(EquipmentActorClass);
	if (IsValid(OwnerSkeletalMesh) == true)
	{
		PWEquipmentActorBase->AttachToComponent(OwnerSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		PWEquipmentActorBase->SetOwner(GetOwner());
	}
}
