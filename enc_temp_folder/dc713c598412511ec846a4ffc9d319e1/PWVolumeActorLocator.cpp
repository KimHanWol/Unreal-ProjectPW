// LINK

//Default
#include "PWVolumeActorLocator.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"

void APWVolumeActorLocator::SetOccupiedVolume(class APWVolumeActorBase* InOccupiedVolume)
{
	OccupiedVolume = InOccupiedVolume;
}

TSubclassOf<class APWVolumeActorBase> APWVolumeActorLocator::GetOccuiedVolumeClass()
{
	if (IsValid(OccupiedVolume) == false)
	{
		return nullptr;
	}

	return OccupiedVolume->GetClass();
}

bool APWVolumeActorLocator::IsSpawnable()
{
	if (IsOccupied() == true)
	{
		return false;
	}

	if (IsValid(BoundMeshComponent) == false)
	{
		ensure(false);
		return false;
	}

	// 캐릭터나 볼륨이랑 겹치지 않아야 스폰 가능
	TArray<AActor*> OverlappingActors;
	BoundMeshComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (IsValid(Cast<APWPlayerCharacter>(OverlappingActor)) == true || IsValid(Cast<APWVolumeActorBase>(OverlappingActor)) == true)
		{
			return false;
		}
	}

	return true;
}

void APWVolumeActorLocator::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//do nothing
}