// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Components/ActorComponent.h"

//Default
#include "PWEquipmentComponent.generated.h"


UCLASS()
class PROJECTPW_API UPWEquipmentComponent: public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void InitializeComponent() override;

private:

	void SpawnEquipmentActor();

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APWEquipmentActorBase> EquipmentActorClass;

	UPROPERTY(Transient)
	USkeletalMeshComponent* OwnerSkeletalMesh;
};