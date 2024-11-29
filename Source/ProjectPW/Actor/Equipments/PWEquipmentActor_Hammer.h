// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"

//Default
#include "PWEquipmentActor_Hammer.generated.h"


UCLASS()
class PROJECTPW_API APWEquipmentActor_Hammer: public APWEquipmentActorBase
{
	GENERATED_BODY()

protected:

	APWEquipmentActor_Hammer(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsInteractableActor(AActor* TargetActor) override;

public:

	//Smash
	virtual void Execute_Main_Triggered() override;

	//Build
	virtual void Execute_Sub_Triggered() override;
	virtual void Execute_Sub_Completed() override;

private:

	UPROPERTY(Transient)
	TSoftObjectPtr<UAnimMontage> Montage_HammerSmash;

	UPROPERTY(Transient)
	TSoftObjectPtr<UAnimMontage> Montage_Build;

	UPROPERTY(EditDefaultsOnly)
	float BuildDistance = 400.f;

	UPROPERTY(Transient)
	bool bBuildPreviewMode = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BuildObjectClass;

	UPROPERTY(Transient)
	AActor* BuildPreviewObject;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInstance* BuildPreviewMI;
};
