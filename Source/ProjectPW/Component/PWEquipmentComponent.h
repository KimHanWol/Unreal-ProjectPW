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

protected:

	virtual void BeginPlay() override;

public:

	void Execute();

private:

	void SpawnEquipmentActor();

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APWEquipmentActorBase> EquipmentActorClass;

	UPROPERTY(Transient)
	USkeletalMeshComponent* OwnerSkeletalMesh;

	UPROPERTY(Transient)
	class APWEquipmentActorBase* SpawnedEquipmentActor;
};