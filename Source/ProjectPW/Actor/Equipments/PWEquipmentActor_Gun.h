// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"

//Default
#include "PWEquipmentActor_Gun.generated.h"


UCLASS()
class PROJECTPW_API APWEquipmentActor_Gun: public APWEquipmentActorBase
{
	GENERATED_BODY()

protected:
		
	virtual void BeginPlay() override;

public:

	//Fire
	virtual void Execute() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 1000.f;
};
