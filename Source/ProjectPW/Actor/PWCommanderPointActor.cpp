// LINK

//Default
#include "PWCommanderPointActor.h"

//Engine
#include "Components/StaticMeshComponent.h"

//Game

APWCommanderPointActor::APWCommanderPointActor()
{
	SpawnAreaLocationActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnAreaLocation"));
	SpawnAreaLocationActor->SetupAttachment(RootComponent);
}

FTransform APWCommanderPointActor::GetSpawnAreaTransform()
{
	if (ensure(IsValid(SpawnAreaLocationActor) == true))
	{
		return SpawnAreaLocationActor->GetComponentTransform();
	}

	return FTransform();
}


