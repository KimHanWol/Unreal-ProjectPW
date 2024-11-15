// LINK

//Default
#include "PWEquipmentActorBase.h"

//Engine

//Game

APWEquipmentActorBase::APWEquipmentActorBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());
}