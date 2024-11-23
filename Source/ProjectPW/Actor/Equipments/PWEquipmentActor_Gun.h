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

public:

	//Fire
	virtual void Execute_Main_Triggered() override;
	virtual void Execute_Main_Completed() override;

	//ADS
	virtual void Execute_Sub_Triggered() override;
	virtual void Execute_Sub_Completed() override;

private:

	void EnableADS(bool bEnabled);

	virtual bool IsInteractableActor(AActor* TargetActor) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> ImpactEffect;

	UPROPERTY(Transient)
	bool bIsADSAnimPlaying = false;
};
