// LINK

//Default
#include "PWVolumeActorLocator.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"

void APWVolumeActorLocator::SetIsOccupied(bool bInOccupied)
{
	bIsOccupied = bInOccupied;
}

bool APWVolumeActorLocator::IsSpawnable()
{
	if (bIsOccupied == true)
	{
		return false;
	}

	if (IsValid(BoundMeshComponent) == false)
	{
		ensure(false);
		return false;
	}

	// 캐릭터가 겹쳐있지 않아야 스폰 가능
	TArray<AActor*> OverlappingActors;
	BoundMeshComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (IsValid(Cast<APWPlayerCharacter>(OverlappingActor)) == true)
		{
			return false;
		}
	}

	return true;
}
