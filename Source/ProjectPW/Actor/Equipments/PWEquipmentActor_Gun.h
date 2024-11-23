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

	APWEquipmentActor_Gun(const FObjectInitializer& ObjectInitializer);

protected:
		
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	//Fire
	virtual void Execute_Main_Triggered() override;
	virtual void Execute_Main_Completed() override;

	//ADS
	virtual void Execute_Sub_Triggered() override;
	virtual void Execute_Sub_Completed() override;

private:

	void EnableADS(bool bEnabled);

	bool EqiupmentActorLineTrace(FHitResult& OutHitResult, FRotator& OutViewPointRotation);

	void OnPlayerPossesssed(APawn* PossessedPawn, bool bIsCommander);

private:

	//TODO: Particle Table 같은 곳으로 이전
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSoftObjectPtr<class UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 1000.f;

	UPROPERTY(Transient)
	bool bIsADSAnimPlaying = false;

	//Weather RMB is pressed
	UPROPERTY(Transient)
	bool bIsSubExecuting = false;

	//타겟을 겨냥하고 있는가?
	UPROPERTY(Transient)
	bool bIsTargetOn = false;
};
