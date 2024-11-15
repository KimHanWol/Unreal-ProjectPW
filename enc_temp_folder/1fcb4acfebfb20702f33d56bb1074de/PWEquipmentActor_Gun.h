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
	virtual void Execute_Main() override;

	//ADS
	virtual void Execute_Sub() override;

private:

	//TODO: Particle Table 같은 곳으로 이전
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> ImpactEffect;

	//TODO: Character AnimSet 같은 곳으로 이전
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TSoftObjectPtr<class UAnimMontage> Montage_ADS;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 1000.f;
};
