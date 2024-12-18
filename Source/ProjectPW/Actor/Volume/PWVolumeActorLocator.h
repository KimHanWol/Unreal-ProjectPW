// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Volume/PWVolumeActorBase.h"

//Default
#include "PWVolumeActorLocator.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActorLocator : public APWVolumeActorBase
{
	GENERATED_BODY()

public:

	bool IsSpawnable();

	void SetOccupiedVolume(class APWVolumeActorBase* InOccupiedVolume);
	TSubclassOf<class APWVolumeActorBase> GetOccuiedVolumeClass();

	bool IsOccupied() const { return IsValid(OccupiedVolume) == true; }

protected:

	virtual void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(Transient)
	class APWVolumeActorBase* OccupiedVolume;

};
